#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERR,      // ← было ERROR, теперь ERR (чтобы не конфликтовать с Windows)
    FATAL
};

class LogMessage {
public:
    LogMessage(LogLevel level, const std::string& serviceName, const std::string& message,
               const std::string& stackTrace = "");

    std::string getTimestamp() const;
    LogLevel getLevel() const;
    std::string getServiceName() const;
    std::string getHostName() const;
    std::string getRequestId() const;
    std::string getMessage() const;
    std::string getStackTrace() const;

    std::string toJson() const;
    std::string toString() const;

private:
    std::string timestamp_;
    LogLevel level_;
    std::string serviceName_;
    std::string hostName_;
    std::string requestId_;
    std::string message_;
    std::string stackTrace_;

    static std::string getCurrentTimestamp();
    static std::string getCurrentHostName();
};

std::string logLevelToString(LogLevel level);