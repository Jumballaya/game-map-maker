#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <vector>

enum class LogType
{
  INFO,
  WARNING,
  ERROR,
};

enum class LogLevel
{
  DEBUG,
  DEV,
  RELEASE,
};

struct LogEntry
{
  LogType type;
  std::string message;
};

class Logger
{
public:
  static LogLevel level;

  static std::vector<LogEntry> messages;

  static void Log(const std::string &message);
  static void Error(const std::string &message);
  static void Warn(const std::string &message);
};

#endif