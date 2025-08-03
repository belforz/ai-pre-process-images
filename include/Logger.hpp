#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

/**
 * @enum LogLevel
 * @brief Represents the severity level of a log message.
 * 
 * - INFO: Informational messages.
 * - WARNING: Warnings about potential issues.
 * - ERROR: Error messages indicating failures.
 * - DEBUG: Debugging information.
 * - SECTION: Special section/separator in logs.
 */
enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG,
    SECTION  // Novo tipo para separadores
};

/**
 * @class Logger
 * @brief Handles logging messages with different severity levels, colors, and emojis.
 * 
 * The Logger class provides a unified interface for logging messages to the console (or file),
 * supporting colored output and emojis for better visualization, including in Docker containers.
 */
class Logger {
public:
    /**
     * @brief Logs a message with the specified severity level.
     *
     * This function logs messages to the console (with color and emoji), or to a file depending on configuration.
     * 
     * @param message The message to log.
     * @param level The severity level of the message (e.g., INFO, WARNING, ERROR, DEBUG, SECTION).
     * @param id Optional identifier for the log entry.
     * @return void
     */
    void log(const std::string &message, LogLevel level, const std::string &id = "");

private:
    /**
     * @brief Returns the ANSI color code for a given log level.
     * @param level The log level.
     * @return ANSI color code as a string.
     */
    std::string getColor(LogLevel level);

    /**
     * @brief Returns an emoji for a given log level.
     * @param level The log level.
     * @return Emoji as a string.
     */
    std::string getEmoji(LogLevel level);
};

extern Logger logger;

#endif // LOGGER_HPP
