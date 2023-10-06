#include "YingLongPCH.h"

#include <fstream>

#include "Config.h"
#include "utils/Path.h"

namespace YingLong {

    const toml::table& Config::ReadOnly(const std::string& SceneName)
    {
        MergeConfigData();
        auto FindResult = MergedConfigs.find(SceneName);
        if (FindResult != MergedConfigs.end())
        {
            return FindResult->second.Data;
        }
        return MergedProjectConfig.Data;
    }

    const toml::table& Config::ReadOnlyWithUser(const std::string& SceneName)
    {
        MergeConfigData();
        auto FindResult = MergedConfigsWithUser.find(SceneName);
        if (FindResult != MergedConfigsWithUser.end())
        {
            return FindResult->second.Data;
        }
        return MergedProjectConfig.Data;
    }

    bool Config::Writable(toml::table& OutTable, ConfigLayer Layer, const std::string& SceneName)
    {
        ConfigTableInfo* Info =nullptr;
        if (Layer == ConfigLayer::Engine) { Info = &EngineConfig; }
        if (Layer == ConfigLayer::Project) { Info = &ProjectConfig; }
        if (Layer == ConfigLayer::Scene && SceneConfigs.find(SceneName) != SceneConfigs.end())
        {
            Info = &SceneConfigs[SceneName];
        }
        if (Layer == ConfigLayer::ProjectUser) { Info = &ProjectUserConfig; }
        if (Layer == ConfigLayer::SceneUser && SceneUserConfigs.find(SceneName) != SceneUserConfigs.end())
        {
            Info = &SceneUserConfigs[SceneName];
        }
        if (Info)
        {
            Info->Dirty = true;
            OutTable = Info->Data;
            return true;
        }
        return false;
    }

    bool Config::LoadSceneConfig(ConfigLayer Layer, const std::string& SceneName)
    {
        bool WithUser = Layer == ConfigLayer::SceneUser;
        toml::table tbl;
        if (!ParseFile(Path::SceneConfigFile(SceneName, WithUser), tbl))
        {
            return false;
        }
        if (WithUser)
        {
            SceneUserConfigs[SceneName] = { true, std::move(tbl) };
        }
        else
        {
            SceneConfigs[SceneName] = { true, std::move(tbl) };
        }
        return true;
    }

    void Config::Flush()
    {
        if (EngineConfig.Dirty) { SaveFile(Path::EngineConfigFile(), EngineConfig.Data); }
        if (ProjectConfig.Dirty) { SaveFile(Path::ProjectConfigFile(), ProjectConfig.Data); }
        for (auto& SceneConfig : SceneConfigs)
        {
            if (SceneConfig.second.Dirty)
            { 
                SaveFile(Path::SceneConfigFile(SceneConfig.first, false), SceneConfig.second.Data);
            }
        }
        if (ProjectUserConfig.Dirty) { SaveFile(Path::ProjectUserConfigFile(), ProjectUserConfig.Data); }
        for (auto& SceneUserConfig : SceneUserConfigs)
        {
            if (SceneUserConfig.second.Dirty)
            {
                SaveFile(Path::SceneConfigFile(SceneUserConfig.first, true), SceneUserConfig.second.Data);
            }
        }
    }

    bool Config::ParseFile(const std::string& Filepath, toml::table& OutTable)
    {
        try
        {
            OutTable = std::move(toml::parse_file(Filepath));
            return true;
        }
        catch (const toml::parse_error& err)
        {
            std::cerr << "Toml Parsing failed:\n" << err << "\n";
            return false;
        }
    }

    bool Config::SaveFile(const std::string& Filepath, toml::table& InTable)
    {
        std::ofstream ConfigFile(Filepath, std::ios::out);
        if (ConfigFile.is_open())
        {
            ConfigFile << InTable;
            ConfigFile.close();
            return true;
        }
        else
        {
            std::cerr << "Save config failed:\n" << Filepath << "\n";
            return false;
        }
    }

    void Config::MergeConfigData()
    {
        // Merge default engine and project config
        bool IsProjectConfigDirty = false;
        if (EngineConfig.Dirty || ProjectConfig.Dirty)
        {
            IsProjectConfigDirty = true;
            MergedProjectConfig = { true, EngineConfig.Data };
            MergeTable(MergedProjectConfig.Data, ProjectConfig.Data);
            EngineConfig.Dirty = false;
            ProjectConfig.Dirty = false;
        }

        // Merge default scene configs
        for (auto& SceneConfig : SceneConfigs)
        {
            if (IsProjectConfigDirty || SceneConfig.second.Dirty)
            {
                MergedConfigs[SceneConfig.first] = { true, MergedProjectConfig.Data };
                MergeTable(MergedConfigs[SceneConfig.first].Data, SceneConfig.second.Data);
                SceneConfig.second.Dirty = false;
            }
        }

        // Merge user project config
        for (auto& SceneConfig : MergedConfigs)
        {
            if (ProjectUserConfig.Dirty || SceneConfig.second.Dirty)
            {
                MergedConfigsWithUser[SceneConfig.first] = { true, SceneConfig.second.Data };
                MergeTable(MergedConfigsWithUser[SceneConfig.first].Data, ProjectUserConfig.Data);
                SceneConfig.second.Dirty = false;
            }
        }

        // Merge user scene configs
        for (auto& SceneConfig : SceneUserConfigs)
        {
            if (SceneConfig.second.Dirty)
            {
                auto FindResult = MergedConfigsWithUser.find(SceneConfig.first);
                if (FindResult != MergedConfigsWithUser.end())
                {
                    MergeTable(FindResult->second.Data, SceneConfig.second.Data);
                }
                else
                {
                    MergedConfigsWithUser[SceneConfig.first] = { false, MergedProjectConfig.Data };
                    MergeTable(MergedConfigsWithUser[SceneConfig.first].Data, ProjectUserConfig.Data);
                    MergeTable(MergedConfigsWithUser[SceneConfig.first].Data, SceneConfig.second.Data);
                }
            }
        }
    }

    void Config::MergeTable(toml::table& BaseTable, const toml::table& CoverTable, uint32 RecursionDepth)
    {
        static uint32 MaxRecursionDepth = 64;
        for (auto& Elem : CoverTable)
        {
            bool IsReplacePerfix = false;
            std::string Key(Elem.first.str());
            if (Key.at(0) == '_')
            {
                IsReplacePerfix = true;
                Key = Key.substr(1);
            }
            auto FindInBase = BaseTable.find(Key);
            if (FindInBase == BaseTable.end() || IsReplacePerfix)
            {
                BaseTable.insert_or_assign(Key, Elem.second);
            }
            else if (FindInBase->second.is_table() && Elem.second.is_table())
            {
                MergeTable(*FindInBase->second.as_table(), *Elem.second.as_table(), RecursionDepth + 1);
            }
            else if (FindInBase->second.is_array() && Elem.second.is_array())
            {
                toml::array& BaseArray = *FindInBase->second.as_array();
                const toml::array& CoverArray = *Elem.second.as_array();
                BaseArray.insert(BaseArray.end(), CoverArray.begin(), CoverArray.end());
            }
            else if (FindInBase->second.type() == Elem.second.type())
            {
                BaseTable.insert_or_assign(Key, Elem.second);
            }
            else
            {
                std::cerr << "Can not merge value of key: " << Key <<
                    ". Base type is: " << FindInBase->second.type() <<
                    ", Cover type is: " << Elem.second.type();
            }
        }
    }

    Config::Config()
    {
        if (ParseFile(Path::EngineConfigFile(), EngineConfig.Data)) { EngineConfig.Dirty = true; }
        if (ParseFile(Path::ProjectConfigFile(), ProjectConfig.Data)) { ProjectConfig.Dirty = true; }
        if (ParseFile(Path::ProjectUserConfigFile(), ProjectUserConfig.Data)) { ProjectUserConfig.Dirty = true; }
    }
}

