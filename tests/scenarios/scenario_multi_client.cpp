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

    std::cout << "=== СЦЕНАРИЙ 5: Работа с несколькими клиентами ===\n\n";

    LogServer& server = LogServer::getInstance();
    server.start(8084);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    LoggerClient c1("127.0.0.1", 8084);
    LoggerClient c2("127.0.0.1", 8084);
    LoggerClient c3("127.0.0.1", 8084);

    c1.connect();
    c2.connect();
    c3.connect();

    c1.logInfo("AuthService", "Клиент 1: Вход выполнен");
    c2.logWarning("Monitor", "Клиент 2: Низкий заряд");
    c3.logError("DB", "Клиент 3: Ошибка базы");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    c1.disconnect();
    c2.disconnect();
    c3.disconnect();
    server.stop();

    std::cout << "Сценарий 5 завершён.\n";
    return 0;
}