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

    std::cout << "=== СЦЕНАРИЙ 2: Ротация файлов по размеру ===\n\n";

    LogServer& server = LogServer::getInstance();
    server.start(8082);
    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    LoggerClient client("127.0.0.1", 8082);
    client.connect();

    std::cout << "Генерируем большую нагрузку для вызова ротации...\n";

    for (int i = 0; i < 250; ++i) {
        client.logInfo("LoadTest", "Большое тестовое сообщение для ротации #" + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    client.disconnect();
    server.stop();

    std::cout << "\nСценарий 2 завершён.\n";
    std::cout << "Проверьте папку logs/ — должны появиться файлы с суффиксами _001, _002 и т.д.\n";

    return 0;
}
