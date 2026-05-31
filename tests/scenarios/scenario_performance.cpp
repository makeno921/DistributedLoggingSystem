#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
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

    std::cout << "=== СЦЕНАРИЙ 9: Производительность при большой нагрузке ===\n\n";

    LogServer& server = LogServer::getInstance();
    if (!server.start(8090)) {
        std::cerr << "[ОШИБКА] Не удалось запустить сервер!\n";
        return 1;
    }
    std::cout << "[OK] Сервер запущен на порту 8090\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(800));

    LoggerClient client("127.0.0.1", 8090);
    if (!client.connect()) {
        std::cerr << "[ОШИБКА] Не удалось подключить клиента!\n";
        server.stop();
        return 1;
    }
    std::cout << "[OK] Клиент подключён\n\n";

    std::cout << "Отправка 500 логов (тест производительности)...\n";

    const std::vector<std::string> services = {
        "AuthService", "DatabaseService", "PaymentService",
        "Monitoring", "UserService", "OrderService", "CacheService"
    };

    int totalLogs = 500;
    for (int i = 0; i < totalLogs; ++i) {
        std::string service = services[i % services.size()];

        if (i % 10 == 0) {
            client.logWarning(service, "Нагрузка #" + std::to_string(i));
        } else if (i % 25 == 0) {
            client.logError(service, "Ошибка под нагрузкой #" + std::to_string(i));
        } else {
            client.logInfo(service, "Сообщение #" + std::to_string(i));
        }

        // Небольшая задержка, чтобы не перегружать сеть и буферы
        if (i % 80 == 0 && i > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(3)); // даём время на обработку

    client.disconnect();
    server.stop();

    std::cout << "\n=== СЦЕНАРИЙ 9 успешно завершён ===\n";
    std::cout << "Отправлено ровно " << totalLogs << " логов.\n";
    std::cout << "Проверьте папку logs/ — должна произойти ротация файлов.\n";

    return 0;
}