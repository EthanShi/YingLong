
#include "YingLongPCH.h"

#include "Log.h"
#include "utils/Path.h"
#include "Config.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace YingLong {

    void Log::SetLogFileName(const std::string& FileName)
    {
        m_FullFileName = Path::ProjectLogFileName(FileName);
    }

    spdlog::level::level_enum Log::GetLogLevelFromString(const std::string& InString)
    {
        static std::unordered_map<std::string, spdlog::level::level_enum> StringToLogLevelMap;
        if (StringToLogLevelMap.empty())
        {
            StringToLogLevelMap["trace"] = spdlog::level::level_enum::trace;
            StringToLogLevelMap["debug"] = spdlog::level::level_enum::debug;
            StringToLogLevelMap["info"] = spdlog::level::level_enum::info;
            StringToLogLevelMap["warn"] = spdlog::level::level_enum::warn;
            StringToLogLevelMap["err"] = spdlog::level::level_enum::err;
            StringToLogLevelMap["critical"] = spdlog::level::level_enum::critical;
        }

        auto FindResult = StringToLogLevelMap.find(InString);
        if (FindResult != StringToLogLevelMap.end())
        {
            return FindResult->second;
        }
        return spdlog::level::n_levels;
    }

	LoggerBase::LoggerBase(const std::string& LoggerName)
	{
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto rotating_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(Log::Instance().GetFileName());
        std::vector<spdlog::sink_ptr> sinks{ stdout_sink, rotating_sink };
        auto logger = std::make_shared<spdlog::async_logger>(LoggerName, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        logger->set_pattern("[%H:%M:%S %z] [%n] [%^-%l-%$] %v");
        spdlog::register_logger(logger);
        m_logger = spdlog::get(LoggerName);

        // Read log level from config
        auto ConfigData = Config::Instance().ReadOnly();
        const std::string& LevelStr = ConfigData["Log"]["Levels"][LoggerName].value_or("info");
        auto Level = Log::GetLogLevelFromString(LevelStr);
        m_logger->set_level(Level);
	}

}