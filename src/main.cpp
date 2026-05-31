#include "LogMessage.h"
#include "LoggerClient.h"
#include "LogServer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <atomic>

#ifdef _WIN32
#include <windows.h>
#endif

std::atomic<bool> running(true);
std::atomic<bool> generationEnabled(false);

void generateLogs(LoggerClient* client) {
    int counter = 0;
    while (running) {
        if (generationEnabled) {
            counter++;
            client->logInfo("Тестовое сообщение #" + std::to_string(counter), "TestService");
            client->logDebug("Отладочная информация #" + std::to_string(counter), "TestService");
            client->logWarning("Предупреждение #" + std::to_string(counter), "TestService");
            client->logError("Ошибка #" + std::to_string(counter), "", "TestService");
            client->logFatal("Критическая ошибка #" + std::to_string(counter), "", "TestService");
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    std::cout << "=== Distributed Logging System (DLS) ===\n\n";

    LogServer& server = LogServer::getInstance();
    if (!server.start(8080)) {
        std::cerr << "Не удалось запустить сервер!\n";
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    LoggerClient client("127.0.0.1", 8080);

    if (client.connect()) {
        std::cout << "\n=== Тестирование ===\n\n";

        client.logInfo("Приложение успешно запущено", "AuthService");
        client.logDebug("Пользователь вошёл в систему", "AuthService");
        client.logWarning("Низкий заряд батареи на сервере", "Monitoring");
        client.logError("Не удалось подключиться к базе данных", "", "DatabaseService");
        client.logFatal("Критическая ошибка: переполнение стека", "", "PaymentService");
    }

    std::thread logGenerator(generateLogs, &client);
    logGenerator.detach();

    std::cout << "\n=== Фильтрация ===\n";
    std::cout << "live          - сброс фильтров\n";
    std::cout << "live debug    - только DEBUG\n";
    std::cout << "live info     - только INFO\n";
    std::cout << "live warning  - только WARNING\n";
    std::cout << "live error    - только ERROR\n";
    std::cout << "live fatal    - только FATAL\n";
    std::cout << "startgen      - включить генерацию\n";
    std::cout << "stopgen       - выключить генерацию\n";
    std::cout << "exit          - выход\n\n";

    std::string cmd;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, cmd);

        if (cmd == "exit" || cmd == "quit") {
            running = false;
            break;
        }
        else if (cmd == "live") server.enableLiveView();
        else if (cmd == "live debug") server.setFilterLevel(LogLevel::DEBUG);
        else if (cmd == "live info") server.setFilterLevel(LogLevel::INFO);
        else if (cmd == "live warning") server.setFilterLevel(LogLevel::WARNING);
        else if (cmd == "live error") server.setFilterLevel(LogLevel::ERR);
        else if (cmd == "live fatal") server.setFilterLevel(LogLevel::FATAL);
        else if (cmd == "startgen") {
            generationEnabled = true;
            std::cout << "[System] Генерация включена\n";
        }
        else if (cmd == "stopgen") {
            generationEnabled = false;
            std::cout << "[System] Генерация выключена\n";
        }
        else std::cout << "Неизвестная команда\n";
    }

    server.stop();
    std::cout << "Программа завершена.\n";
    return 0;
}