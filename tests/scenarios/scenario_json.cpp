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

    std::cout << "=== СЦЕНАРИЙ 7: Тестирование JSON формата ===\n\n";

    LogServer& server = LogServer::getInstance();
    server.start(8086);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    LoggerClient client("127.0.0.1", 8086);
    client.connect();

    client.logInfo("AuthService", "Тест JSON");
    client.logError("Database", "Ошибка подключения", "Stack trace example");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    client.disconnect();
    server.stop();

    std::cout << "Сценарий 7 завершён.\n";
    return 0;
}