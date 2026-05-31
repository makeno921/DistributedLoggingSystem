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

    std::cout << "=== СЦЕНАРИЙ 1: Полный цикл работы системы ===\n\n";

    LogServer& server = LogServer::getInstance();
    server.start(8080);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    LoggerClient client("127.0.0.1", 8080);
    client.connect();

    client.logInfo("AuthService", "Приложение успешно запущено");
    client.logDebug("AuthService", "Пользователь вошёл в систему");
    client.logWarning("Monitoring", "Низкий заряд батареи");
    client.logError("DatabaseService", "Не удалось подключиться к базе данных");
    client.logFatal("PaymentService", "Критическая ошибка: переполнение стека");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    client.disconnect();
    server.stop();

    std::cout << "Сценарий 1 завершён успешно.\n";
    return 0;
}
