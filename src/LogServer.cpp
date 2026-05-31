#include "LogServer.h"
#include <iostream>

LogServer::LogServer() : port_(8080), running_(false), liveViewEnabled_(true), showAllLogs_(true), filterLevel_(LogLevel::DEBUG) {}

LogServer::~LogServer() {
    stop();
}

LogServer& LogServer::getInstance() {
    static LogServer instance;
    return instance;
}

bool LogServer::start(int port) {
    port_ = port;
    running_ = true;

    std::cout << "[LogServer] Starting server on port " << port << "...\n";

    if (!listenerSocket_.listen(port)) {
        std::cerr << "[LogServer] Failed to start listening!\n";
        return false;
    }

    acceptThread_ = std::thread(&LogServer::acceptClients, this);

    std::cout << "[LogServer] Server started successfully!\n";
    return true;
}

void LogServer::stop() {
    if (!running_) return;

    running_ = false;
    listenerSocket_.disconnect();   // закрываем слушающий сокет

    if (acceptThread_.joinable()) {
        acceptThread_.join();       // ждём завершения потока
    }

    std::cout << "[LogServer] Server stopped.\n";
}

void LogServer::acceptClients() {
    while (running_) {
        auto client = std::make_unique<TcpConnection>();
        if (listenerSocket_.acceptConnection(*client)) {
            std::lock_guard<std::mutex> lock(clientsMutex_);
            clients_.push_back(std::move(client));
            std::cout << "[LogServer] New client connected!\n";

            std::thread(&LogServer::processClient, this, clients_.back().get()).detach();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void LogServer::processClient(TcpConnection* client) {
    while (running_ && client->isConnected()) {
        std::string data = client->receive();
        if (data.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        size_t pos = 0;
        while ((pos = data.find("{\"timestamp", pos)) != std::string::npos) {
            size_t end = data.find("}", pos);
            if (end != std::string::npos) {
                std::string singleJson = data.substr(pos, end - pos + 1);
                LogMessage log = parseJsonLog(singleJson);
                broadcastLog(log);
                pos = end + 1;
            } else {
                break;
            }
        }
    }
}

LogMessage LogServer::parseJsonLog(const std::string& jsonData) {
    std::string message = extractJsonValue(jsonData, "message");
    std::string service = extractJsonValue(jsonData, "service");
    std::string levelStr = extractJsonValue(jsonData, "level");

    LogLevel level = LogLevel::INFO;
    if (levelStr == "DEBUG") level = LogLevel::DEBUG;
    else if (levelStr == "WARNING") level = LogLevel::WARNING;
    else if (levelStr == "ERROR") level = LogLevel::ERR;
    else if (levelStr == "FATAL") level = LogLevel::FATAL;

    return LogMessage(level, service.empty() ? "RemoteClient" : service, message);
}

std::string LogServer::extractJsonValue(const std::string& json, const std::string& key) {
    size_t pos = json.find("\"" + key + "\":\"");
    if (pos == std::string::npos) return "";

    pos += key.length() + 4;
    size_t end = json.find("\"", pos);
    if (end == std::string::npos) return "";

    return json.substr(pos, end - pos);
}

void LogServer::broadcastLog(const LogMessage& msg) {
    // Основная логика фильтрации
    bool shouldShow = false;

    if (showAllLogs_) {
        shouldShow = true;                    // Если включен "все логи"
    } else if (msg.getLevel() == filterLevel_) {
        shouldShow = true;                    // Если уровень совпадает с фильтром
    }

    if (shouldShow && liveViewEnabled_) {
        std::cout << "[LIVE] SAVED: " << msg.toString() << std::endl;
    }

    // Сохраняем в файл ТОЛЬКО если проходит фильтр
    if (shouldShow) {
        storage_.save(msg);
    }
}

void LogServer::enableLiveView() {
    liveViewEnabled_ = true;
    showAllLogs_ = true;
    std::cout << "[LogServer] Отображение включено (все логи)\n";
}

void LogServer::disableLiveView() {
    liveViewEnabled_ = false;
    std::cout << "[LogServer] Отображение выключено\n";
}

void LogServer::setFilterLevel(LogLevel level) {
    liveViewEnabled_ = true;
    showAllLogs_ = false;
    filterLevel_ = level;
    std::cout << "[LogServer] Фильтр: " << logLevelToString(level) << "\n";
}