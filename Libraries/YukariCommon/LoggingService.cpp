/** @file */

#include "LoggingService.h"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <spdlog/sinks/base_sink.h>

namespace Yukari
{
namespace Common
{
  spdlog::level::level_enum LoggingService::GetLogLevelFromStr(const std::string &levelStr)
  {
    auto end = spdlog::level::level_names + 7;
    auto pos = std::find(spdlog::level::level_names, end, levelStr);
    if (pos == end)
      throw std::runtime_error("Invalid log level: \"" + levelStr + "\"");
    spdlog::level::level_enum level = (spdlog::level::level_enum)(pos - spdlog::level::level_names);
    return level;
  }

  void LoggingService::EnsureLogDirectoryExists(const std::string &filename)
  {
    boost::filesystem::path p(filename);
    boost::filesystem::path parentDir = p.parent_path();
    boost::filesystem::create_directories(parentDir);
  }

  LoggingService::LoggingService()
      : m_sink(std::make_shared<NamedDistLogSink_mt>())
  {
#ifndef NDEBUG
    auto stdoutSink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
#else
    auto stdoutSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#endif
    stdoutSink->set_level(spdlog::level::trace);
    m_sink->addSink("console", stdoutSink);

#ifndef NDEBUG
    m_sink->set_level(spdlog::level::trace);
#endif

    getLogger("LoggingService")->info("Init logging service");
  }

  LoggingService::~LoggingService()
  {
    flush();
  }

  void LoggingService::configure(boost::program_options::variables_map &args)
  {
    if (args.count("loglevel"))
    {
      auto level = GetLogLevelFromStr(args["loglevel"].as<std::string>());
      setLevel(level);
    }
  }

  LoggingService::Logger LoggingService::getLogger(const std::string &name)
  {
    auto logger = spdlog::get(name);
    if (!logger)
    {
      logger = std::make_shared<spdlog::logger>(name, m_sink);

      /* Default to trace level for all loggers */
      logger->set_level(spdlog::level::trace);

      /* Register the logger */
      spdlog::register_logger(logger);
    }

    return logger;
  }
}
}
