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

    std::cout << "=== СЦЕНАРИЙ 4: Обработка stackTrace ===\n\n";

    LogServer& server = LogServer::getInstance();
    server.start(8083);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    LoggerClient client("127.0.0.1", 8083);
    client.connect();

    client.logFatal("PaymentService", "Критическая ошибка оплаты",
                   "at PaymentProcessor.cpp:145\nat Transaction.cpp:89\nat main.cpp:234");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    client.disconnect();
    server.stop();

    std::cout << "Сценарий 4 завершён.\n";
    return 0;
}