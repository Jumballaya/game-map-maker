#include "Logger.h"
#include <iostream>
#include <ctime>
#include <chrono>

std::vector<LogEntry> Logger::messages;

std::string timestamp()
{
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string out(30, '\0');
  std::strftime(&out[0], out.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
  return out;
}

void Logger::Log(const std::string &message)
{
  LogEntry entry;
  entry.type = LOG_INFO;
  entry.message = "LOG: [" + timestamp() + "]: " + message;
  std::cout << "\x1B[92m" << entry.message << "\033[0m" << std::endl;
  messages.push_back(entry);
}

void Logger::Error(const std::string &message)
{
  LogEntry entry;
  entry.type = LOG_ERROR;
  entry.message = "ERR: [" + timestamp() + "]: " + message;
  std::cerr << "\x1B[91m" << entry.message << "\033[0m" << std::endl;
  messages.push_back(entry);
}

void Logger::Warn(const std::string &message)
{
  LogEntry entry;
  entry.type = LOG_WARNING;
  entry.message = "WRN: [" + timestamp() + "]: " + message;
  std::cout << "\x1B[93m" << entry.message << "\033[0m" << std::endl;
  messages.push_back(entry);
}