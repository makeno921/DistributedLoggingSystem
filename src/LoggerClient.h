#pragma once
#include "LogMessage.h"
#include "TcpConnection.h"
#include <string>
#include <memory>

class LoggerClient {
public:
    explicit LoggerClient(const std::string& serverIp = "127.0.0.1", int serverPort = 8080,
                         const std::string& authToken = "");

    ~LoggerClient();

    bool connect();
    void disconnect();

    bool logDebug(const std::string& message, const std::string& serviceName = "default");
    bool logInfo(const std::string& message, const std::string& serviceName = "default");
    bool logWarning(const std::string& message, const std::string& serviceName = "default");
    bool logError(const std::string& message, const std::string& stackTrace = "",
                  const std::string& serviceName = "default");
    bool logFatal(const std::string& message, const std::string& stackTrace = "",
                  const std::string& serviceName = "default");

    bool sendLog(const LogMessage& logMsg);

private:
    std::string serverIp_;
    int serverPort_;
    std::string authToken_;
    bool isConnected_;

    std::unique_ptr<TcpConnection> tcpConnection_;
};