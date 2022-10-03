#include "YingLongPCH.h"
#include "Path.h"

namespace YingLong {

	namespace Path {

		std::string EngineRootDir()
		{
			return std::string();
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

		std::string ProjectPersistenceDir()
		{
			return "persistence/";
		}

		std::string ProjectLogDir()
		{
			return ProjectPersistenceDir() + "log/";
		}

		std::string ProjectLogFileName(const std::string& FileName)
		{
			auto Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
			uint64 Timestamp = Now.count();
			std::string TimestampString = std::to_string(Timestamp);
			return ProjectLogDir() + FileName + "-" + TimestampString + ".log";
		}
	}

}

