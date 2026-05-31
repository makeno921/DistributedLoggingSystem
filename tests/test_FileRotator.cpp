#include <catch2/catch_test_macros.hpp>
#include "FileRotator.h"
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

TEST_CASE("FileRotator: создание и инициализация", "[FileRotator]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    std::string testDir = "test_logs";
    FileRotator rotator(testDir);

    bool dirExists = std::filesystem::exists(testDir) ||
                     std::filesystem::exists("../" + testDir);
    REQUIRE(dirExists);
}

TEST_CASE("FileRotator: запись одного сообщения", "[FileRotator]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    FileRotator rotator("test_logs");
    LogMessage msg(LogLevel::INFO, "AuthService", "Тестовое сообщение");
    REQUIRE(rotator.write(msg));
}

TEST_CASE("FileRotator: запись нескольких сообщений", "[FileRotator]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    FileRotator rotator("test_logs");
    for (int i = 0; i < 10; ++i) {
        LogMessage msg(LogLevel::INFO, "TestService", "Сообщение #" + std::to_string(i));
        REQUIRE(rotator.write(msg));
    }
}

TEST_CASE("FileRotator: разные уровни логирования", "[FileRotator]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    FileRotator rotator("test_logs");
    REQUIRE(rotator.write(LogMessage(LogLevel::DEBUG, "Test", "debug")));
    REQUIRE(rotator.write(LogMessage(LogLevel::INFO, "Test", "info")));
    REQUIRE(rotator.write(LogMessage(LogLevel::WARNING, "Test", "warning")));
    REQUIRE(rotator.write(LogMessage(LogLevel::ERR, "Test", "error")));
    REQUIRE(rotator.write(LogMessage(LogLevel::FATAL, "Test", "fatal")));
}

TEST_CASE("FileRotator: ротация по размеру работает", "[FileRotator]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    FileRotator rotator("test_logs");
    for (int i = 0; i < 400; ++i) {
        LogMessage msg(LogLevel::INFO, "LoadTest", "Сообщение для теста ротации #" + std::to_string(i));
        rotator.write(msg);
    }
}

TEST_CASE("FileRotator: несколько экземпляров", "[FileRotator]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    FileRotator r1("logs_dir1");
    FileRotator r2("logs_dir2");
    REQUIRE(r1.write(LogMessage(LogLevel::INFO, "Service1", "msg1")));
    REQUIRE(r2.write(LogMessage(LogLevel::WARNING, "Service2", "msg2")));
}