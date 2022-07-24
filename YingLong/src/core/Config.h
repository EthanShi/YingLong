#pragma once

#include <map>
#include <string>

#include "Macros.h"
#include "toml/toml.hpp"

namespace YingLong {

	class YINGLONG_API Config
	{
	public:
		/*
		* We can divide config layers to two parts: Developer's config(Engine, Project, Scene) and User's config(ProjectUser, SceneUser)
		* Config layers graph like this:
		* -----------------------------------------------------------------
		* High
		*  |	SceneUser1		SceneUser2		......		--
		*  |												 | -- Configs with user
		*  |					ProjectUser					--
		*  |
		*  |	Scene1			Scene2			......		--
		*  |					Project						 | -- Configs
		*  |					Engine						--
		* Low
		* -----------------------------------------------------------------
		* 
		* [Merged config]
		* Higher layer configs will cover lower layer configs
		* If some configs enable user to modify, read from "ReadOnlyWithUser()", other configs just read from "ReadOnly()" which ignore user configs.
		* 
		* [Remove low layer's config]
		* There is no way to remove low layer's config by higher layers, just edit the config file.
		*/
		enum class ConfigLayer
		{
			Engine,
			Project,
			Scene,
			ProjectUser,
			SceneUser
		};

	public:
		static Config& Instance()
		{
			static Config instance;
			return instance;
		}

		const toml::table& ReadOnly();
		const toml::table& ReadOnlyWithUser();
		bool Writable(toml::table& OutTable, ConfigLayer Layer, const std::string& SceneName = "");
		bool LoadSceneConfig(ConfigLayer Layer, const std::string& SceneName);
		void Flush();

	private:
		struct ConfigTableInfo {
			bool Dirty = false;
			toml::table Data;
		};

		ConfigTableInfo m_EngineConfig;
		ConfigTableInfo m_ProjectConfig;
		std::map<std::string, ConfigTableInfo> m_SceneConfigs;
		ConfigTableInfo m_ProjectUserConfig;
		std::map<std::string, ConfigTableInfo> m_SceneUserConfigs;

		// Merged configs
		toml::table m_Merged;
		toml::table m_MergedWithUser;

	private:
		Config();
		~Config() {}
		Config(const Config&) = delete;
		Config& operator=(const Config&) = delete;

		bool ParseFile(const std::string& Filepath, toml::table& OutTable);
		bool SaveFile(const std::string& Filepath, toml::table& InTable);
		void MergeConfigData();
		void MergeTable(toml::table& BaseTable, const toml::table& CoverTable, uint32 RecursionDepth = 0);
	};

}