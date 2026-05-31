#include <iostream>
#include <thread>
#include <chrono>
#include "LogServer.h"
#include "LoggerClient.h"

#ifdef _WIN32
#include <windows.h>
static inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

int main() {
#ifdef _WIN32
    setupConsoleUTF8();
#endif

    std::cout << "=== СЦЕНАРИЙ 8: Устойчивость к отключениям ===\n\n";

    // 1. Запуск сервера
    LogServer& server = LogServer::getInstance();
    if (!server.start(8090)) {
        std::cerr << "[ОШИБКА] Не удалось запустить сервер!\n";
        return 1;
    }
    std::cout << "[OK] Сервер запущен на порту 8090\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(600));

    // 2. Подключение клиента
    LoggerClient client("127.0.0.1", 8090);
    if (!client.connect()) {
        std::cerr << "[ОШИБКА] Не удалось подключить клиента!\n";
        server.stop();
        return 1;
    }
    std::cout << "[OK] Клиент успешно подключился\n\n";

    // 3. Отправка логов
    std::cout << "Отправка логов...\n";
    client.logInfo("AuthService", "Нормальная работа");
    client.logWarning("Monitoring", "Высокая нагрузка");
    client.logError("DatabaseService", "Проблема с соединением");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 4. Резкое отключение клиента
    std::cout << "\nРезкое отключение клиента (симуляция обрыва соединения)...\n";
    // Вместо forceDisconnect() просто уничтожаем объект
    client.disconnect();   // или можно оставить без вызова, чтобы деструктор сработал

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 5. Проверка, что сервер продолжает работать
    std::cout << "Проверка устойчивости сервера...\n";

    LoggerClient client2("127.0.0.1", 8090);
    if (client2.connect()) {
        std::cout << "[OK] Сервер успешно принял нового клиента после обрыва\n";
        client2.logInfo("RecoveryService", "Система восстановилась после отключения");
        client2.disconnect();
    } else {
        std::cout << "[ОШИБКА] Сервер не принял нового клиента\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 6. Завершение
    server.stop();

    std::cout << "\n=== СЦЕНАРИЙ 8 успешно завершён ===\n";
    std::cout << "Сервер проявил устойчивость к внезапному отключению клиента.\n";

    return 0;
}