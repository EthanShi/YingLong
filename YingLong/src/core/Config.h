#pragma once

#include <unordered_map>
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
		* [Replace perfix]
		* If Higher layer's table/array type config key has perfix '_', merged data will use higher config data.
		* Or, merged data will merge higher to lower data for table type. Append higher data to lower data for array type.
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

		const toml::table& ReadOnly(const std::string& SceneName = "");
		const toml::table& ReadOnlyWithUser(const std::string& SceneName = "");
		bool Writable(toml::table& OutTable, ConfigLayer Layer, const std::string& SceneName = "");
		bool LoadSceneConfig(ConfigLayer Layer, const std::string& SceneName);
		void Flush();

	private:
		struct ConfigTableInfo {
			bool Dirty = false;
			toml::table Data;
		};

		ConfigTableInfo EngineConfig;
		ConfigTableInfo ProjectConfig;
		std::unordered_map<std::string, ConfigTableInfo> SceneConfigs;
		ConfigTableInfo ProjectUserConfig;
		std::unordered_map<std::string, ConfigTableInfo> SceneUserConfigs;

		// Merged configs
		ConfigTableInfo MergedProjectConfig;
		std::unordered_map<std::string, ConfigTableInfo> MergedConfigs;
		std::unordered_map<std::string, ConfigTableInfo> MergedConfigsWithUser;

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