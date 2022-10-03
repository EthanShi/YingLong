#pragma once

#include <string>
#include <chrono>

#include "scene/Scene.h"

namespace YingLong {

	namespace Path {

		std::string EngineRootDir();
		std::string EngineConfigDir();
		std::string EngineConfigFile();
		std::string ProjectConfigDir();
		std::string ProjectConfigFile();
		std::string ProjectUserConfigFile();
		std::string SceneConfigFile(const std::string& SceneName, bool WithUser);
		std::string SceneConfigFile(Scene* InScene, bool WithUser);
		std::string ProjectPersistenceDir();
		std::string ProjectLogDir();
		std::string ProjectLogFileName(const std::string& FileName);

	}

}