#include "Enemy.h"
#include "Knight.h"
#include "Audio.h"
#include "Scene.h"

using namespace agp;

Enemy::Enemy(Scene* scene, const RectF& rect, Sprite* sprite, int layer)
	: DynamicObject(scene, rect, sprite, layer)
{
	_smashable = true;
	_dying = false;
	_facingDir = Direction::LEFT;
}

bool Enemy::collision(CollidableObject* with, Direction fromDir)
{
	Knight* knight = dynamic_cast<Knight*>(with);

	if (knight)
	{
		if (_smashable && knight->invincible())
			smash();
		else
			knight->hurt();

		return true;
	}

	return false;
}

void Enemy::smash()
{
	_dying = true;
	_yGravityForce = 25;
	_vel.y = -8;
	_collidable = false;
	_flip = SDL_FLIP_VERTICAL;
	Audio::instance()->playSound("kick");

	schedule("die-smash", 2, [this]() {_scene->killObject(this); });
}