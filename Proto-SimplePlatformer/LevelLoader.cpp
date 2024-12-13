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
#include "Knight.h"
#include "HammerBrother.h"
#include "Lift.h"
#include "Trigger.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "mathUtils.h"
#include "OverlayScene.h"

using namespace agp;

LevelLoader::LevelLoader()
{
	// e.g. load level data from disk
}

void LevelLoader::loadJson(
	PlatformerGameScene* world,
	const std::string& jsonPath,
	Knight* link)
{
	std::ifstream f(jsonPath);
	if (!f.is_open())
		return;

	nlohmann::json jRoot = nlohmann::json::parse(f);
	std::vector <std::string> _categories = jRoot["categories"].get<std::vector<std::string>>();
	std::vector<nlohmann::json> jsonObjects = jRoot["objects"].get<std::vector<nlohmann::json>>();

	// portals with matching names = portals to be connected
	//std::map<std::string, std::vector<Portal*>> portals;

	for (auto& jObj : jsonObjects)
	{
		int category = jObj["category"];
		std::string name = jObj["name"];

		if (jObj.contains("rect") || jObj.contains("rotRect"))
		{
			RotatedRectF rrect;

			if (jObj.contains("rect"))
			{
				RectF rect;
				rect.pos.x = jObj["rect"]["x"];
				rect.pos.y = jObj["rect"]["y"];
				rect.size.x = jObj["rect"]["width"];
				rect.size.y = jObj["rect"]["height"];
				rect.yUp = jObj["rect"]["yUp"];
				rrect = rect;
			}
			else
			{
				rrect.center.x = jObj["rotRect"]["cx"];
				rrect.center.y = jObj["rotRect"]["cy"];
				rrect.size.x = jObj["rotRect"]["width"];
				rrect.size.y = jObj["rotRect"]["height"];
				rrect.angle = deg2rad(float(jObj["rotRect"]["angle"]));
				rrect.yUp = jObj["rotRect"]["yUp"];
			}

			if (_categories[category] == "Terrain")
				new StaticObject(world, rrect, nullptr, 1);
			else if (_categories[category] == "Breakable")
				new StaticObject(world, rrect, nullptr, 1);
		}
		else if (jObj.contains("multiline"))
		{
			std::vector<nlohmann::json> jsonPoints = jObj["multiline"].get<std::vector<nlohmann::json>>();
			for (int i = 0; i < jsonPoints.size() - 1; i++)
			{
				float x1 = jsonPoints[i]["x"];
				float y1 = jsonPoints[i]["y"];
				float x2 = jsonPoints[i + 1]["x"];
				float y2 = jsonPoints[i + 1]["y"];
				LineF line(x1, y1, x2, y2);
				new StaticObject(world, RotatedRectF(line, 0.1f, false), nullptr, 2);
			}
		}
	}

	// connect paired portals
	/*for (auto& pair : portals)
		if (pair.second.size() == 2)
		{
			pair.second[0]->setDestination(pair.second[1]);
			pair.second[1]->setDestination(pair.second[0]);
		}
		else
			std::cerr << "Found " << pair.second.size() << " portals with name " << pair.first << ": expected 2\n";*/

	f.close();
}

Scene* LevelLoader::load(const std::string& name)
{
	SpriteFactory* spriteLoader = SpriteFactory::instance();

	if (name == "overworld")
	{
		PlatformerGameScene* world = new PlatformerGameScene(RectF(0, 0, 427, 80), { 16,16 }, 1 / 100.0f);
		new RenderableObject(world, RectF(0, 0, 427, 80), spriteLoader->get("overworld"));

		Knight* player = new Knight(world, PointF(7, 49));
		world->setPlayer(player);

		// Caricamento oggetti dal file json generato dal level editor
		loadJson(world, std::string(SOURCE_DIR) + "EditorScene.json", player);


		world->addBackgroundScene(new OverlayScene(world, spriteLoader->get("sky_bg"), { 0,0 }, false, { 16,16 }, { 0,1 }));
		world->addBackgroundScene(new OverlayScene(world, spriteLoader->get("castle_bg"), { 0.2f, 1 }, true, { 32,32 }, { -15,-2 }));
		world->addBackgroundScene(new OverlayScene(world, spriteLoader->get("trees1_bg"), { 0.4f, 1 }, true, { 32,32 }, { 0,-2 }));
		world->addBackgroundScene(new OverlayScene(world, spriteLoader->get("trees2_bg"), { 0.6f, 1 }, true, { 32,32 }, {0,-2}));


		return world;
	}
	else
	{
		std::cerr << "Unrecognized game scene name \"" << name << "\"\n";
		return nullptr;
	}
}