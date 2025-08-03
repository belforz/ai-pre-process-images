#include "Logger.hpp"
#include <iostream>
#include <chrono>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#endif

Logger logger;

std::string currentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buf[26];
    std::tm *tm_info = std::localtime(&now_time);
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    return std::string(buf);
}

std::string Logger::getColor(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return "\033[1;34m"; // Blue
        case LogLevel::WARNING: return "\033[1;33m"; // Yellow
        case LogLevel::ERROR:   return "\033[1;31m"; // Red
        case LogLevel::DEBUG:   return "\033[1;36m"; // Cian
        case LogLevel::SECTION: return "\033[1;35m"; // Purple
        // Default color (reset)
        default: return "\033[0m";
    }
}

std::string Logger::getEmoji(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:    return "ℹ️ ";
        case LogLevel::WARNING: return "⚠️ ";
        case LogLevel::ERROR:   return "❌ ";
        case LogLevel::DEBUG:   return "🐛 ";
        case LogLevel::SECTION: return "🔷 ";
        default: return "";
    }
}

void Logger::log(const std::string &message, LogLevel level, const std::string &id) {
#ifdef _WIN32
    // Enable ANSI escape codes on Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif

    std::string timestamp = currentTime();
    std::string levelStr;
    switch (level) {
        case LogLevel::INFO:    levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARN"; break;
        case LogLevel::ERROR:   levelStr = "ERROR"; break;
        case LogLevel::DEBUG:   levelStr = "DEBUG"; break;
        case LogLevel::SECTION: levelStr = "SECTION"; break;
    }

    std::string logId = id.empty() ? "" : "[ID: " + id + "] ";
    std::string color = getColor(level);
    std::string emoji = getEmoji(level);

    // Section blocks
    if (level == LogLevel::SECTION) {
        std::cout << "\n" << color << "=========== " << emoji << message << " ===========\033[0m\n";
    } else {
        std::cout << color
                  << "[" << timestamp << "] [" << levelStr << "] "
                  << logId << emoji << message
                  << "\033[0m" << std::endl;
    }
}
