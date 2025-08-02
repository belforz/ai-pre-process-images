#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>

enum class LogLevel {
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

class Logger {
    public:
        /**
         * @brief Logs a message with the specified severity level.
         *
         * This function logs messages to the console or a file, depending on the configuration.
         * @enum type of LogLevel >>> INFO, WARNING, ERROR, DEBUG
         * @param message The message to log.
         * @param LogLevel The severity level of the message (e.g., INFO, WARNING, ERROR).
         * @param id identifier for the log entry
            * @return void
            */
         void log(const std::string &message, LogLevel level, const std::string &id = "") ;
            // Implementation of logging logic goes here
            // This could include writing to a file, console output, etc.
    private:
        // Private members for currentTime

};

extern Logger logger;

#endif // LOGGER_HPP
