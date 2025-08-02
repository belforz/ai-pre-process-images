#include "Logger.hpp"

#include <iostream>
#include <chrono>
#include <ctime>

Logger logger;

std::string currentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buf[26];
    std::tm* tm_info = std::localtime(&now_time);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buf);
}

void Logger::log(const std::string &message, LogLevel level, const std::string &id) {
    std::string levelStr;
    switch (level) {
        case LogLevel::INFO: levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARN"; break;
        case LogLevel::ERROR: levelStr = "ERROR"; break;
        case LogLevel::DEBUG: levelStr = "DEBUG"; break;
    }

    std::string timestamp = currentTime();
    std::string logId = id.empty() ? "" : "[ID: " + id + "] ";

    std::cout << "[" << timestamp << "] [" << levelStr << "] " << logId << message << std::endl;
}
