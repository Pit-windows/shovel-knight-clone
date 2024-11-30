// ----------------------------------------------------------------
// From "Algorithms and Game Programming" in C++ by Alessandro Bria
// Copyright (C) 2024 Alessandro Bria (a.bria@unicas.it). 
// All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include <string>
#include "Singleton.h"

namespace agp
{
	class PlatformerGameScene;
	class Scene;
	class LevelLoader;
	class Knight;
}

// LevelLoader (singleton)
// - provides game scene creation methods
class agp::LevelLoader : public Singleton<LevelLoader>
{
	friend class Singleton<LevelLoader>;

	protected:

		// constructor accessible only to Singleton (thanks to friend declaration)
		LevelLoader();

		void loadJson(
			PlatformerGameScene* world,
			const std::string& jsonPath,
			Knight* link);

	public:

		Scene* load(const std::string& name);
};
