// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "LevelLoader.h"
#include "SpriteFactory.h"
#include "RenderableObject.h"
#include "StaticObject.h"
#include "PlatformerGameScene.h"
#include "Mario.h"
#include "HammerBrother.h"
#include "Lift.h"
#include "Trigger.h"
#include <iostream>

using namespace agp;

LevelLoader::LevelLoader()
{
	// e.g. load level data from disk
}

Scene* LevelLoader::load(const std::string& name)
{
	SpriteFactory* spriteLoader = SpriteFactory::instance();

	if (name == "overworld")
	{
		PlatformerGameScene* world = new PlatformerGameScene(RectF(0, 0, 427, 80), { 16,16 }, 1 / 100.0f);
		world->setBackgroundColor(Color(92, 148, 252));
		
		new RenderableObject(world, RectF(0, 0, 427, 80), spriteLoader->get("overworld"));
		new StaticObject(world, RectF(1, 51, 38, 1), nullptr, 1);
		new StaticObject(world, RectF(39, 49, 4, 1), nullptr, 1);
		new StaticObject(world, RectF(43, 37, 3, 1), nullptr, 1);
		new StaticObject(world, RectF(46, 49, 4, 1), nullptr, 1);
		new StaticObject(world, RectF(43, 47, 3, 1), nullptr, 1);
		new StaticObject(world, RectF(50, 51, 14, 1), nullptr, 1);
		new StaticObject(world, RectF(53, 47, 8, 1), nullptr, 1);
		new StaticObject(world, RectF(64, 47, 1, 5), nullptr, 1);
		new StaticObject(world, RectF(49, 50, 1, 2), nullptr, 1);
		new StaticObject(world, RectF(45, 48, 1, 2), nullptr, 1);
		new StaticObject(world, RectF(43, 48, 1, 2), nullptr, 1);
		new StaticObject(world, RectF(39, 50, 1, 2), nullptr, 1);

		Mario* player = new Mario(world, PointF(8, 50));
		world->setPlayer(player);

		return world;
	}
	else
	{
		std::cerr << "Unrecognized game scene name \"" << name << "\"\n";
		return nullptr;
	}
}