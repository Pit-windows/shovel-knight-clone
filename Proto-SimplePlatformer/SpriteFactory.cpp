// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SpriteFactory.h"
#include "SDL_image.h"
#include "graphicsUtils.h"
#include "sdlUtils.h"
#include "Window.h"
#include "AnimatedSprite.h"
#include "TiledSprite.h"
#include "FilledSprite.h"
#include "Game.h"
#include <iostream>

using namespace agp;

SpriteFactory::SpriteFactory()
{
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return;
	}

	SDL_Renderer* renderer = Game::instance()->window()->renderer();
	_spriteSheets["sky_bg"] = loadTexture(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/sky_bg.png");
	_spriteSheets["castle_bg"] = loadTexture(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/castle_prova.png");
	_spriteSheets["trees1_bg"] = loadTexture(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/trees1_prova.png");
	_spriteSheets["trees2_bg"] = loadTexture(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/trees2_prova.png");
	_spriteSheets["mario"] = loadTextureAutoDetect(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/mario.png", _autoTiles["mario"], {27, 89, 153}, { 147, 187, 236 });
	_spriteSheets["enemies"] = loadTextureAutoDetect(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/enemies.png", _autoTiles["enemies"], { 27, 89, 153 }, { 147, 187, 236 }, 17);
	_spriteSheets["hud"] = loadTexture(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/hud.png", { 147, 187, 236 });
	_spriteSheets["tiles"] = loadTextureAutoDetect(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/stage_tiles.png", _autoTiles["tiles"], { 27, 89, 153 }, { 147, 187, 236 }, 5, true, false);
	_spriteSheets["overworld"] = loadTexture(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/overworld.png");
	_spriteSheets["knight"] = loadTextureAutoDetect(renderer, std::string(SOURCE_DIR) + "/assets/" + "sprites/knight.png", _autoTiles["knight"], { 0, 128, 128 }, { 0, 255, 0 }, 5, true, false, true);
}

// anchors
static RectI hud_letter(519, 254, 8, 8);
static RectI hud_number(519, 263, 8, 8);
static RectI hud_letter_disabled(519, 366, 8, 8);
static RectI hud_number_disabled(519, 375, 8, 8);
static RectI hud_coin(519, 289, 8, 8);

Sprite* SpriteFactory::get(const std::string& id)
{
	std::vector< RectI> rects;

	// overworld
	if (id == "overworld")
		return new Sprite(_spriteSheets["overworld"]);

	// background
	else if (id == "sky_bg")
		return new Sprite(_spriteSheets["sky_bg"]);
	else if (id == "castle_bg")
		return new FilledSprite(_spriteSheets["castle_bg"]);
	else if (id == "trees1_bg")
		return new FilledSprite(_spriteSheets["trees1_bg"]);
	else if (id == "trees2_bg")
		return new FilledSprite(_spriteSheets["trees2_bg"]);


	// single-frame sprites
	else if (id == "welcome")
		return new Sprite(_spriteSheets["hud"], RectI(1, 2 + 16 * 2, 16 * 16, 13 * 16));
	else if (id == "gameover")
		return new Sprite(_spriteSheets["hud"], RectI(260, 253, 16 * 16, 15 * 16));
	else if (id == "knight_stand")
		return new Sprite(_spriteSheets["knight"], _autoTiles["knight"][0][0]);
	else if (id == "knight_jump")
		return new Sprite(_spriteSheets["knight"], _autoTiles["knight"][5][0]);
	else if (id == "knight_skid")
		return new Sprite(_spriteSheets["knight"], _autoTiles["knight"][4][0]);
	else if (id == "mario_die")
		return new Sprite(_spriteSheets["mario"], _autoTiles["mario"][0][1]);
	else if (id == "mario_attack")
		return new Sprite(_spriteSheets["mario"], _autoTiles["mario"][0][4]);
	else if (id == "hammer")
		return new Sprite(_spriteSheets["enemies"], _autoTiles["enemies"][0][25]);
	else if (id == "hammer_brother_jump")
		return new Sprite(_spriteSheets["enemies"], _autoTiles["enemies"][0][24]);
	else if (id == "platform")
		return new FilledSprite(_spriteSheets["tiles"], _autoTiles["tiles"][8][33]);

	// animated sprites
	else if (id == "knight_walk")
		return new AnimatedSprite(_spriteSheets["knight"], { _autoTiles["knight"][3].begin(), _autoTiles["knight"][3].begin() + 5 }, 10);
	else if (id == "mario_run")
		return new AnimatedSprite(_spriteSheets["mario"], { _autoTiles["mario"][0].begin() + 2, _autoTiles["mario"][0].begin() + 5 }, 20);
	else if (id == "box")
		return new AnimatedSprite(_spriteSheets["tiles"], { _autoTiles["tiles"][3].begin(), _autoTiles["tiles"][3].begin() + 3 }, 5, {0, 1, 2, 1, 0});
	else if (id == "hud_coin")
	{
		rects.push_back(moveBy(hud_coin, 0, 0));
		rects.push_back(moveBy(hud_coin, 1, 0, 8, 8));
		rects.push_back(moveBy(hud_coin, 2, 0, 8, 8));
		rects.push_back(moveBy(hud_coin, 1, 0, 8, 8));
		rects.push_back(moveBy(hud_coin, 0, 0));
		rects.push_back(moveBy(hud_coin, 0, 0));
		return new AnimatedSprite(_spriteSheets["hud"], rects, 6);
	}
	else if (id == "hammer_brother_walk")
		return new AnimatedSprite(_spriteSheets["enemies"], { _autoTiles["enemies"][0].begin() + 21, _autoTiles["enemies"][0].begin() + 23 }, 8);
	else if (id == "hammer_brother_throw")
		return new AnimatedSprite(_spriteSheets["enemies"], { _autoTiles["enemies"][0].begin() + 23, _autoTiles["enemies"][0].begin() + 25 }, 8);
	else if (id == "link_sword")
		return new AnimatedSprite(_spriteSheets["link"], { _autoTiles["link"][9].begin(), _autoTiles["link"][9].begin() + 9 }, 22, { 0, 1, 2, 3, 4, 5, 5, 6, 7, 8, 8 }, 1);

	// tiled sprites
	if (id.compare(0, 4, "pipe") == 0)
	{
		std::string digits = id.substr(4);
		int length = std::stoi(digits);
		rects.push_back(_autoTiles["tiles"][2][6]);
		rects.push_back(_autoTiles["tiles"][2][7]);
		for (int i = 0; i < length; i++)
		{
			rects.push_back(_autoTiles["tiles"][3][6]);
			rects.push_back(_autoTiles["tiles"][3][7]);
		}
		return new TiledSprite(_spriteSheets["tiles"], rects);
	}


	else
	{
		std::cerr << "Cannot find sprite \"" << id << "\"\n";
		return nullptr;
	}
}

Sprite* SpriteFactory::getText(std::string text, const Vec2Df& size, int fillN, char fillChar, bool enabled)
{
	std::vector< RectI> tiles;

	if (fillN)
		while (text.size() != fillN)
			text = fillChar + text;

	RectI& number_anchor = enabled ? hud_number : hud_number_disabled;
	RectI& letter_anchor = enabled ? hud_letter : hud_letter_disabled;

	for (auto& c : text)
	{
		if(isdigit(c))
			tiles.push_back(moveBy(number_anchor, c - '0', 0, 8, 8));
		else if (isalpha(c))
			tiles.push_back(moveBy(letter_anchor, toupper(c) - 'A', 0, 8, 8));
		else if (c == '-')
			tiles.push_back(moveBy(number_anchor, 10, 0, 8, 8));
		else if (c == '*')
			tiles.push_back(moveBy(number_anchor, 11, 0, 8, 8));
		else if (c == '!')
			tiles.push_back(moveBy(number_anchor, 12, 0, 8, 8));
		else if (c == '©')
			tiles.push_back(moveBy(number_anchor, 13, 0, 8, 8));
		else if (c == '.')
			tiles.push_back(moveBy(number_anchor, 14, 0, 8, 8));
		else
			tiles.push_back(moveBy(hud_letter, 0, -5, 8, 8));	// empty space
	}

	return new TiledSprite(_spriteSheets["hud"], tiles, size);
}