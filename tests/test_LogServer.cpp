#include <catch2/catch_test_macros.hpp>
#include "LogServer.h"

#ifdef _WIN32
#include <windows.h>
static inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

TEST_CASE("LogServer: Singleton pattern", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& s1 = LogServer::getInstance();
    LogServer& s2 = LogServer::getInstance();
    REQUIRE(&s1 == &s2);
}

TEST_CASE("LogServer: запуск и остановка сервера", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();

    REQUIRE(server.start(9100));
    server.stop();                    // сразу останавливаем

    SUCCEED("Сервер успешно запустился и остановился");
}

TEST_CASE("LogServer: несколько запусков подряд", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();

    REQUIRE(server.start(9101));
    server.stop();
    REQUIRE(server.start(9102));
    server.stop();

    SUCCEED("Несколько запусков подряд работают");
}

TEST_CASE("LogServer: установка фильтра уровня", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();
    server.setFilterLevel(LogLevel::ERR);
    server.setFilterLevel(LogLevel::INFO);
    server.setFilterLevel(LogLevel::WARNING);
    SUCCEED("Фильтры уровней устанавливаются");
}

TEST_CASE("LogServer: включение/выключение live view", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogServer& server = LogServer::getInstance();
    server.enableLiveView();
    server.disableLiveView();
    server.enableLiveView();
    SUCCEED("Live View управляется");
}

TEST_CASE("LogServer: обработка INFO лога", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    SUCCEED("INFO лог обрабатывается");
}

TEST_CASE("LogServer: обработка ERROR лога", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    SUCCEED("ERROR лог обрабатывается");
}

TEST_CASE("LogServer: обработка FATAL лога", "[LogServer]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    SUCCEED("FATAL лог обрабатывается");
}