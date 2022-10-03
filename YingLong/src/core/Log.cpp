
#include "YingLongPCH.h"

#include "Log.h"
#include "utils/Path.h"

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace YingLong {

    void Log::SetLogFileName(const std::string& FileName)
    {
        m_FullFileName = Path::ProjectLogFileName(FileName);
    }

	LoggerBase::LoggerBase(const std::string& LoggerName)
	{
        spdlog::init_thread_pool(8192, 1);
        auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto rotating_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(Log::Instance().GetFileName());
        std::vector<spdlog::sink_ptr> sinks{ stdout_sink, rotating_sink };
        auto logger = std::make_shared<spdlog::async_logger>(LoggerName, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        logger->set_pattern("[%H:%M:%S %z] [%n] [%^-%l-%$] %v");
        spdlog::register_logger(logger);
        m_logger = spdlog::get(LoggerName);
	}

}