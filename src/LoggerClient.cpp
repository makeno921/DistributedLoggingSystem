#include "LoggerClient.h"
#include <iostream>

LoggerClient::LoggerClient(const std::string& serverIp, int serverPort,
                           const std::string& authToken)
    : serverIp_(serverIp)
    , serverPort_(serverPort)
    , authToken_(authToken)
    , isConnected_(false)
{
    tcpConnection_ = std::make_unique<TcpConnection>();
}

LoggerClient::~LoggerClient() {
    disconnect();
}

bool LoggerClient::connect() {
    if (tcpConnection_->connect(serverIp_, serverPort_)) {
        isConnected_ = true;
        return true;
    }
    return false;
}

void LoggerClient::disconnect() {
    if (isConnected_) {
        tcpConnection_->disconnect();
        isConnected_ = false;
    }
}

bool LoggerClient::sendLog(const LogMessage& logMsg) {
    if (!isConnected_) {
        return false;
    }
    std::string json = logMsg.toJson();
    return tcpConnection_->send(json);
}

bool LoggerClient::logDebug(const std::string& message, const std::string& serviceName) {
    LogMessage log(LogLevel::DEBUG, serviceName, message);
    return sendLog(log);
}

bool LoggerClient::logInfo(const std::string& message, const std::string& serviceName) {
    LogMessage log(LogLevel::INFO, serviceName, message);
    return sendLog(log);
}

bool LoggerClient::logWarning(const std::string& message, const std::string& serviceName) {
    LogMessage log(LogLevel::WARNING, serviceName, message);
    return sendLog(log);
}

bool LoggerClient::logError(const std::string& message, const std::string& stackTrace,
                            const std::string& serviceName) {
    LogMessage log(LogLevel::ERR, serviceName, message, stackTrace);
    return sendLog(log);
}

bool LoggerClient::logFatal(const std::string& message, const std::string& stackTrace,
                            const std::string& serviceName) {
    LogMessage log(LogLevel::FATAL, serviceName, message, stackTrace);
    return sendLog(log);
}