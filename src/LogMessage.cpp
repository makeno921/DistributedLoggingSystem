#include "LogMessage.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <netdb.h>
#endif

LogMessage::LogMessage(LogLevel level, const std::string& serviceName, const std::string& message,
                       const std::string& stackTrace)
    : level_(level)
    , serviceName_(serviceName)
    , message_(message)
    , stackTrace_(stackTrace)
    , timestamp_(getCurrentTimestamp())
    , hostName_(getCurrentHostName())
    , requestId_(std::to_string(std::chrono::system_clock::now().time_since_epoch().count())) {}

std::string LogMessage::getTimestamp() const { return timestamp_; }
LogLevel LogMessage::getLevel() const { return level_; }
std::string LogMessage::getServiceName() const { return serviceName_; }
std::string LogMessage::getHostName() const { return hostName_; }
std::string LogMessage::getRequestId() const { return requestId_; }
std::string LogMessage::getMessage() const { return message_; }
std::string LogMessage::getStackTrace() const { return stackTrace_; }

std::string LogMessage::toJson() const {
    std::ostringstream oss;
    oss << R"({"timestamp":")" << timestamp_ << R"(","level":")" << logLevelToString(level_)
        << R"(","service":")" << serviceName_ << R"(","host":")" << hostName_
        << R"(","requestId":")" << requestId_ << R"(","message":")" << message_
        << R"(","stackTrace":")" << stackTrace_ << R"("})";
    return oss.str();
}

std::string LogMessage::toString() const {
    return timestamp_ + " [" + logLevelToString(level_) + "] " + serviceName_ + ": " + message_;
}

std::string LogMessage::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&timeT), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string LogMessage::getCurrentHostName() {
    char hostname[256] = "unknown";
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    gethostname(hostname, sizeof(hostname));
    WSACleanup();
#else
    gethostname(hostname, sizeof(hostname));
#endif
    return hostname;
}

std::string logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERR:     return "ERROR";
        case LogLevel::FATAL:   return "FATAL";
        default:                return "UNKNOWN";
    }
}