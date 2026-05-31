#pragma once
#include "LogMessage.h"
#include "LogStorage.h"
#include "TcpConnection.h"
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

class LogServer {
public:
    static LogServer& getInstance();

    bool start(int port = 8080);
    void stop();

    void enableLiveView();               // live — все логи
    void disableLiveView();
    void setFilterLevel(LogLevel level); // live debug, live error и т.д.

private:
    LogServer();
    ~LogServer();

    void acceptClients();
    void processClient(TcpConnection* client);

    LogMessage parseJsonLog(const std::string& jsonData);
    std::string extractJsonValue(const std::string& json, const std::string& key);
    void broadcastLog(const LogMessage& msg);

    int port_;
    std::atomic<bool> running_;
    TcpConnection listenerSocket_;
    std::vector<std::unique_ptr<TcpConnection>> clients_;
    LogStorage storage_;

    std::thread acceptThread_;
    mutable std::mutex clientsMutex_;

    bool liveViewEnabled_ = true;
    bool showAllLogs_ = true;            // true = показывать все логи
    LogLevel filterLevel_ = LogLevel::DEBUG;
};