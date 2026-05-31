#include <catch2/catch_test_macros.hpp>
#include "LogStorage.h"
#include "LogMessage.h"
#include <filesystem>
#include <string>

#ifdef _WIN32
#include <windows.h>

static inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

TEST_CASE("LogStorage: создание объекта", "[LogStorage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    REQUIRE_NOTHROW([]() {
        LogStorage storage;
    }());
}

TEST_CASE("LogStorage: сохранение одного сообщения", "[LogStorage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogStorage storage;

    LogMessage msg(LogLevel::INFO, "AuthService", "Тест сохранения");
    REQUIRE(storage.save(msg));
}

TEST_CASE("LogStorage: сохранение нескольких сообщений", "[LogStorage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogStorage storage;

    for (int i = 0; i < 20; ++i) {
        LogMessage msg(LogLevel::INFO, "Test", "Сообщение " + std::to_string(i));
        REQUIRE(storage.save(msg));
    }
}

TEST_CASE("LogStorage: разные уровни логирования", "[LogStorage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogStorage storage;

    REQUIRE(storage.save(LogMessage(LogLevel::DEBUG, "Test", "debug")));
    REQUIRE(storage.save(LogMessage(LogLevel::INFO, "Test", "info")));
    REQUIRE(storage.save(LogMessage(LogLevel::WARNING, "Test", "warning")));
    REQUIRE(storage.save(LogMessage(LogLevel::ERR, "Test", "error")));
    REQUIRE(storage.save(LogMessage(LogLevel::FATAL, "Test", "fatal")));
}

TEST_CASE("LogStorage: сохранение с stackTrace", "[LogStorage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogStorage storage;

    LogMessage fatal(LogLevel::FATAL, "Payment", "Критическая ошибка", "long stack trace here");
    REQUIRE(storage.save(fatal));
}

TEST_CASE("LogStorage: множественное сохранение", "[LogStorage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogStorage storage;

    for (int i = 0; i < 50; ++i) {
        LogMessage msg(LogLevel::INFO, "StressTest", "msg " + std::to_string(i));
        storage.save(msg);
    }
    SUCCEED("Множественное сохранение прошло без ошибок");
}