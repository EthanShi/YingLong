#include "YingLongPCH.h"

#include <fstream>

#include "Config.h"
#include "utils/Path.h"

namespace YingLong {

    const toml::table& Config::ReadOnly(const std::string& SceneName)
    {
        MergeConfigData();
        auto& FindResult = m_MergedConfigs.find(SceneName);
        if (FindResult != m_MergedConfigs.end())
        {
            return FindResult->second.Data;
        }
        return m_MergedProjectConfig.Data;
    }

    const toml::table& Config::ReadOnlyWithUser(const std::string& SceneName)
    {
        MergeConfigData();
        auto& FindResult = m_MergedConfigsWithUser.find(SceneName);
        if (FindResult != m_MergedConfigsWithUser.end())
        {
            return FindResult->second.Data;
        }
        return m_MergedProjectConfig.Data;
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
        // Merge default engine and project config
        bool IsProjectConfigDirty = false;
        if (m_EngineConfig.Dirty || m_ProjectConfig.Dirty)
        {
            IsProjectConfigDirty = true;
            m_MergedProjectConfig = { true, m_EngineConfig.Data };
            MergeTable(m_MergedProjectConfig.Data, m_ProjectConfig.Data);
            m_EngineConfig.Dirty = false;
            m_ProjectConfig.Dirty = false;
        }

        // Merge default scene configs
        for (auto& SceneConfig : m_SceneConfigs)
        {
            if (IsProjectConfigDirty || SceneConfig.second.Dirty)
            {
                m_MergedConfigs[SceneConfig.first] = { true, m_MergedProjectConfig.Data };
                MergeTable(m_MergedConfigs[SceneConfig.first].Data, SceneConfig.second.Data);
                SceneConfig.second.Dirty = false;
            }
        }

        // Merge user project config
        for (auto& SceneConfig : m_MergedConfigs)
        {
            if (m_ProjectUserConfig.Dirty || SceneConfig.second.Dirty)
            {
                m_MergedConfigsWithUser[SceneConfig.first] = { true, SceneConfig.second.Data };
                MergeTable(m_MergedConfigsWithUser[SceneConfig.first].Data, m_ProjectUserConfig.Data);
                SceneConfig.second.Dirty = false;
            }
        }

        // Merge user scene configs
        for (auto& SceneConfig : m_SceneUserConfigs)
        {
            if (SceneConfig.second.Dirty)
            {
                auto& FindResult = m_MergedConfigsWithUser.find(SceneConfig.first);
                if (FindResult != m_MergedConfigsWithUser.end())
                {
                    MergeTable(FindResult->second.Data, SceneConfig.second.Data);
                }
                else
                {
                    m_MergedConfigsWithUser[SceneConfig.first] = { false, m_MergedProjectConfig.Data };
                    MergeTable(m_MergedConfigsWithUser[SceneConfig.first].Data, m_ProjectUserConfig.Data);
                    MergeTable(m_MergedConfigsWithUser[SceneConfig.first].Data, SceneConfig.second.Data);
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

