#pragma once

#include <string>

#include "scene/Scene.h"

namespace YingLong {

	namespace Path {

		std::string EngineRootDir()
		{
			return "../";
		}

		std::string EngineConfigDir()
		{
			return "../YingLong/config/";
		}

		std::string EngineConfigFile()
		{
			return EngineConfigDir() + "engine.toml";
		}

		std::string ProjectConfigDir()
		{
			return "config/";
		}

		std::string ProjectConfigFile()
		{
			return "config/project.toml";
		}

		std::string ProjectUserConfigFile()
		{
			return "config/projectUser.toml";
		}

		std::string SceneConfigFile(const std::string& SceneName, bool WithUser)
		{
			return ProjectConfigDir() + SceneName + (WithUser ? "User" : "") + ".toml";
		}

		std::string SceneConfigFile(Scene* InScene, bool WithUser)
		{
			if (!InScene)
			{
				return "";
			}
			return SceneConfigFile(InScene->GetName(), WithUser);
		}
	}

}