#include <iostream>
#include <thread>
#include <chrono>
#include "LogServer.h"
#include "LoggerClient.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "=== СЦЕНАРИЙ 3: Фильтрация логов в реальном времени ===\n\n";

    LogServer& server = LogServer::getInstance();
    server.start(8081);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    LoggerClient client("127.0.0.1", 8081);
    client.connect();

    std::cout << "Отправляем логи всех уровней...\n";
    client.logInfo("Auth", "Info сообщение");
    client.logWarning("Monitor", "Warning сообщение");
    client.logError("DB", "Error сообщение");
    client.logFatal("Payment", "Fatal сообщение");

    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    std::cout << "\nВключаем фильтр ERROR...\n";
    server.setFilterLevel(LogLevel::ERR);
    server.enableLiveView();

    client.logError("");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    client.disconnect();
    server.stop();

    std::cout << "\nСценарий 3 завершён.\n";
    return 0;
}
