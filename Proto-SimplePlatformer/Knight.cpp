// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Knight.h"
#include "SpriteFactory.h"
#include "Audio.h"
#include "AnimatedSprite.h"
#include "PlatformerGame.h"
#include "Scene.h"

using namespace agp;

Knight::Knight(Scene* scene, const PointF& pos)
	: DynamicObject(scene, RectF(pos.x + 1 / 16.0f, pos.y, 2, 2), nullptr)
{
	_collider.adjust(0.2f, 0, -0.2f, -1 / 16.0f);
	_fit = false;
	_walking = false;
	_jumping = false;
	_dying = false;
	_dead = false;
	_invincible = false;
	_attacking = false;

	_xLastNonZeroVel = 0;

	_sprites["stand"] = SpriteFactory::instance()->get("knight_stand");
	_sprites["walk"] = SpriteFactory::instance()->get("knight_walk");
	_sprites["skid"] = SpriteFactory::instance()->get("knight_skid");
	_sprites["jump"] = SpriteFactory::instance()->get("knight_jump");
	_sprites["die"] = SpriteFactory::instance()->get("mario_die");
	_sprites["attack"] = SpriteFactory::instance()->get("mario_attack");
	_sprite = _sprites["stand"];
}

void Knight::update(float dt)
{
	// physics
	DynamicObject::update(dt);

	// state logic
	if (_jumping && grounded())
		_jumping = false;
	if (_vel.x != 0 && !_jumping)
		_xLastNonZeroVel = _vel.x;
	_walking = _vel.x != 0;

	// animations
	if (_dying)
		_sprite = _sprites["die"];
	else if (_attacking)
		_sprite = _sprites["attack"];
	else if (_jumping)
		_sprite = _sprites["jump"];
	else if (skidding())
		_sprite = _sprites["skid"];
	else if (_walking)
		_sprite = _sprites["walk"];
	else
		_sprite = _sprites["stand"];

	// x-mirroring
	if ((_vel.x < 0 && !_jumping) || _xLastNonZeroVel < 0)
		_flip = SDL_FLIP_HORIZONTAL;
	else
		_flip = SDL_FLIP_NONE;
}

void Knight::move(Direction dir)
{
	if (_dying || _dead)
		return;

	DynamicObject::move(dir);
}

void Knight::jump(bool on)
{
	if (_dying || _dead)
		return;

	if (on && !midair())
	{
		velAdd(Vec2Df(0, -_yJumpImpulse));

		if (std::abs(_vel.x) < 9)
			_yGravityForce = 25;
		else
			_yGravityForce = 21;

		_jumping = true;
		Audio::instance()->playSound("jump-small");
	}
	else if (!on && midair() && !_dying)
		_yGravityForce = 100;
}

void Knight::run(bool on)
{
	if (midair())
		return;

	if (on)
	{
		_xVelMax = 10;
		_xMoveForce = 13;
	}
	else
	{
		_xVelMax = 6;
		_xMoveForce = 8;
	}
}

void Knight::attack()
{
}

void Knight::die()
{
	if (_dying)
		return;

	_dying = true;
	_collidable = false;
	_yGravityForce = 0;
	_vel = { 0,0 };
	_xDir = Direction::NONE;
	Audio::instance()->haltMusic();
	Audio::instance()->playSound("death");
	dynamic_cast<PlatformerGame*>(Game::instance())->freeze(true);

	schedule("dying", 0.5f, [this]()
		{
			_yGravityForce = 25;
			velAdd(Vec2Df(0, -_yJumpImpulse));
			schedule("die", 3, [this]()
				{
					_dead = true;
					dynamic_cast<PlatformerGame*>(Game::instance())->gameover();
				});
		});
}

void Knight::hurt()
{
	// TODO: powerdown (e.g. if Mario is big, becomes small)
	if (!_invincible)
		die();
}