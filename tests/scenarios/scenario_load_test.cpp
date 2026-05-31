#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
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

    std::cout << "=== СЦЕНАРИЙ 6: Тестирование разных сервисов ===\n\n";

    // 1. Запуск сервера
    LogServer& server = LogServer::getInstance();
    if (!server.start(8086)) {
        std::cerr << "[ОШИБКА] Не удалось запустить сервер!\n";
        return 1;
    }
    std::cout << "[OK] Сервер запущен на порту 8086\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 2. Подключение клиента
    LoggerClient client("127.0.0.1", 8086);
    if (!client.connect()) {
        std::cerr << "[ОШИБКА] Не удалось подключить клиента!\n";
        server.stop();
        return 1;
    }
    std::cout << "[OK] Клиент успешно подключился\n\n";

    // 3. Отправка логов от разных сервисов
    std::cout << "Отправка логов от разных сервисов:\n";

    const std::vector<std::string> services = {
        "AuthService", "DatabaseService", "PaymentService",
        "Monitoring", "UserService", "CacheService",
        "NotificationService", "OrderService"
    };

    for (int i = 0; i < 120; ++i) {
        std::string service = services[i % services.size()];

        if (i % 8 == 0) {
            client.logInfo(service, "Успешная операция #" + std::to_string(i));
        } else if (i % 8 == 1) {
            client.logDebug(service, "Отладочное сообщение #" + std::to_string(i));
        } else if (i % 8 == 3) {
            client.logWarning(service, "Предупреждение в сервисе #" + std::to_string(i));
        } else if (i % 8 == 5) {
            client.logError(service, "Ошибка при обработке #" + std::to_string(i));
        } else {
            client.logInfo(service, "Стандартное сообщение #" + std::to_string(i));
        }

        if (i % 30 == 0 && i > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 4. Завершение
    client.disconnect();
    server.stop();

    std::cout << "\n=== СЦЕНАРИЙ 6 успешно завершён ===\n";
    std::cout << "Логи отправлены от " << services.size() << " разных сервисов.\n";
    std::cout << "Проверьте в файлах логов корректность поля 'service'.\n";

    return 0;
}