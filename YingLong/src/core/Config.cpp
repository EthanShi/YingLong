#include "YingLongPCH.h"

#include <fstream>

#include "Config.h"
#include "utils/Path.h"

namespace YingLong {

    const toml::table& Config::ReadOnly()
    {
        MergeConfigData();
        return m_Merged;
    }

    const toml::table& Config::ReadOnlyWithUser()
    {
        MergeConfigData();
        return m_MergedWithUser;
    }

    bool Config::Writable(toml::table& OutTable, ConfigLayer Layer, const std::string& SceneName)
    {
        ConfigTableInfo* Info =nullptr;
        if (Layer == ConfigLayer::Engine) { Info = &m_EngineConfig; }
        if (Layer == ConfigLayer::Project) { Info = &m_ProjectConfig; }
        if (Layer == ConfigLayer::Scene && m_SceneConfigs.find(SceneName) != m_SceneConfigs.end())
        {
            Info = &m_SceneConfigs[SceneName];
        }
        if (Layer == ConfigLayer::ProjectUser) { Info = &m_ProjectUserConfig; }
        if (Layer == ConfigLayer::SceneUser && m_SceneUserConfigs.find(SceneName) != m_SceneUserConfigs.end())
        {
            Info = &m_SceneUserConfigs[SceneName];
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
            m_SceneUserConfigs[SceneName] = { true, std::move(tbl) };
        }
        else
        {
            m_SceneConfigs[SceneName] = { true, std::move(tbl) };
        }
        return true;
    }

    void Config::Flush()
    {
        if (m_EngineConfig.Dirty) { SaveFile(Path::EngineConfigFile(), m_EngineConfig.Data); }
        if (m_ProjectConfig.Dirty) { SaveFile(Path::ProjectConfigFile(), m_ProjectConfig.Data); }
        for (auto& SceneConfig : m_SceneConfigs)
        {
            if (SceneConfig.second.Dirty)
            { 
                SaveFile(Path::SceneConfigFile(SceneConfig.first, false), SceneConfig.second.Data);
            }
        }
        if (m_ProjectUserConfig.Dirty) { SaveFile(Path::ProjectUserConfigFile(), m_ProjectUserConfig.Data); }
        for (auto& SceneUserConfig : m_SceneUserConfigs)
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
        bool HasDirtySceneConfig = false;
        for (auto& SceneConfig : m_SceneConfigs)
        {
            if (SceneConfig.second.Dirty)
            {
                HasDirtySceneConfig = true;
                break;
            }
        }
        if (m_EngineConfig.Dirty || m_ProjectConfig.Dirty || HasDirtySceneConfig)
        {
            m_Merged = m_EngineConfig.Data;
            m_EngineConfig.Dirty = false;
            MergeTable(m_Merged, m_ProjectConfig.Data);
            m_ProjectConfig.Dirty = false;
            for (auto& SceneConfig : m_SceneConfigs)
            {
                MergeTable(m_Merged, SceneConfig.second.Data);
                SceneConfig.second.Dirty = false;
            }
            m_MergedWithUser = m_Merged;
        }

        bool HasDirtyUserSceneConfig = false;
        for (auto& SceneUserConfig : m_SceneUserConfigs)
        {
            if (SceneUserConfig.second.Dirty)
            {
                HasDirtyUserSceneConfig = true;
                break;
            }
        }
        if (m_ProjectUserConfig.Dirty || HasDirtyUserSceneConfig)
        {
            MergeTable(m_MergedWithUser, m_ProjectUserConfig.Data);
            m_ProjectUserConfig.Dirty = false;
            for (auto& SceneUserConfig : m_SceneUserConfigs)
            {
                MergeTable(m_Merged, SceneUserConfig.second.Data);
                SceneUserConfig.second.Dirty = false;
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
            auto& FindInBase = BaseTable.find(Key);
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
        if (ParseFile(Path::EngineConfigFile(), m_EngineConfig.Data)) { m_EngineConfig.Dirty = true; }
        if (ParseFile(Path::ProjectConfigFile(), m_ProjectConfig.Data)) { m_ProjectConfig.Dirty = true; }
        if (ParseFile(Path::ProjectUserConfigFile(), m_ProjectUserConfig.Data)) { m_ProjectUserConfig.Dirty = true; }
    }
}

