#pragma once

#include <string>
#include "Macros.h"
#include "spdlog/async.h"
#include "spdlog/async_logger.h"

#define DECLARE_LOGGER(LoggerName)\
class LoggerName##Class : public YingLong::LoggerBase\
{\
public:\
	LoggerName##Class() = default;\
	LoggerName##Class(const std::string& Name)\
		: LoggerBase(Name)\
	{}\
};

#define DEFINE_LOGGER(LoggerName) LoggerName##Class LoggerName(#LoggerName);

namespace YingLong {

	class YINGLONG_API Log
	{
	public:
		static Log& Instance()
		{
			static Log LogInstance;
			return LogInstance;
		}

		void SetLogFileName(const std::string& FileName);
		std::string& GetFileName() { return m_FullFileName; }

		static spdlog::level::level_enum GetLogLevelFromString(const std::string& InString);

	private:
		std::string m_FullFileName;

	private:
		Log()
			: m_FullFileName("")
		{
			spdlog::init_thread_pool(8192, 1);
			SetLogFileName("Default");
		}
		~Log() {}
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;
	};

	class YINGLONG_API LoggerBase
	{
	public:
		LoggerBase() = default;
        LoggerBase(const std::string& LoggerName);
		virtual ~LoggerBase() {}

		spdlog::logger& operator()()
		{
			ASSERT(m_logger)
			return *m_logger;
		}

	private:


	private:
		std::shared_ptr<spdlog::logger> m_logger;
	};

}