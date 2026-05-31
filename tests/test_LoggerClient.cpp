#include <catch2/catch_test_macros.hpp>
#include "LoggerClient.h"
#include "LogServer.h"
#include <thread>
#include <chrono>
#include <string>

#ifdef _WIN32
#include <windows.h>

static inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

TEST_CASE("LoggerClient: создание объекта", "[LoggerClient]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    REQUIRE_NOTHROW([]() {
        LoggerClient client("127.0.0.1", 8080);
    }());
}

TEST_CASE("LoggerClient: подключение и отключение", "[LoggerClient]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();
    server.start(8120);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    LoggerClient client("127.0.0.1", 8120);
    REQUIRE(client.connect());
    client.disconnect();
    server.stop();
}

TEST_CASE("LoggerClient: отправка INFO лога", "[LoggerClient]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();
    server.start(8121);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    LoggerClient client("127.0.0.1", 8121);
    REQUIRE(client.connect());
    REQUIRE(client.logInfo("AuthService", "Пользователь вошёл"));
    client.disconnect();
    server.stop();
}

TEST_CASE("LoggerClient: отправка ERROR и FATAL", "[LoggerClient]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();
    server.start(8122);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    LoggerClient client("127.0.0.1", 8122);
    REQUIRE(client.connect());
    REQUIRE(client.logError("DB", "Ошибка подключения"));
    REQUIRE(client.logFatal("Payment", "Критическая ошибка"));
    client.disconnect();
    server.stop();
}

TEST_CASE("LoggerClient: множественная отправка", "[LoggerClient]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();
    server.start(8123);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    LoggerClient client("127.0.0.1", 8123);
    REQUIRE(client.connect());
    for (int i = 0; i < 10; ++i) {
        client.logInfo("Test", "msg " + std::to_string(i));
    }
    client.disconnect();
    server.stop();
}

TEST_CASE("LoggerClient: отключение без подключения", "[LoggerClient]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LoggerClient client("127.0.0.1", 9999);
    client.disconnect(); // не должно падать
    SUCCEED("Корректная обработка отключения");
}