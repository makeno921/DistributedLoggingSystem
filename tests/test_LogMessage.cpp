#include <catch2/catch_test_macros.hpp>
#include "LogMessage.h"
#include <string>

#ifdef _WIN32
#include <windows.h>

static inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

TEST_CASE("LogMessage: создание и базовые геттеры", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage msg(LogLevel::INFO, "AuthService", "Пользователь вошёл в систему");

    REQUIRE(msg.getLevel() == LogLevel::INFO);
    REQUIRE(msg.getServiceName() == "AuthService");
    REQUIRE(msg.getMessage() == "Пользователь вошёл в систему");
}

TEST_CASE("LogMessage: разные уровни логирования", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage debug(LogLevel::DEBUG, "Test", "debug msg");
    LogMessage info(LogLevel::INFO, "Test", "info msg");
    LogMessage warning(LogLevel::WARNING, "Test", "warning msg");
    LogMessage error(LogLevel::ERR, "Test", "error msg");
    LogMessage fatal(LogLevel::FATAL, "Test", "fatal msg");

    REQUIRE(debug.getLevel() == LogLevel::DEBUG);
    REQUIRE(info.getLevel() == LogLevel::INFO);
    REQUIRE(warning.getLevel() == LogLevel::WARNING);
    REQUIRE(error.getLevel() == LogLevel::ERR);
    REQUIRE(fatal.getLevel() == LogLevel::FATAL);
}

TEST_CASE("LogMessage: пустые строки в конструкторе", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage msg(LogLevel::ERR, "", "");
    REQUIRE(msg.getServiceName() == "");
    REQUIRE(msg.getMessage() == "");
}

TEST_CASE("LogMessage: toString() корректный формат", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage msg(LogLevel::WARNING, "DatabaseService", "Не удалось подключиться");
    std::string str = msg.toString();

    REQUIRE(str.find("WARNING") != std::string::npos);
    REQUIRE(str.find("DatabaseService") != std::string::npos);
}

TEST_CASE("LogMessage: toJson() возвращает валидный JSON", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage msg(LogLevel::INFO, "PaymentService", "Платёж прошёл");
    std::string json = msg.toJson();

    REQUIRE(json.find("\"level\":\"INFO\"") != std::string::npos);
    REQUIRE(json.find("\"service\":\"PaymentService\"") != std::string::npos);
}

TEST_CASE("LogMessage: timestamp и requestId генерируются", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage msg1(LogLevel::DEBUG, "Test", "msg1");
    LogMessage msg2(LogLevel::DEBUG, "Test", "msg2");

    REQUIRE(msg1.toJson().find("timestamp") != std::string::npos);
    REQUIRE(msg1.toJson().find("requestId") != std::string::npos);
}

TEST_CASE("LogMessage: stackTrace для ERROR/FATAL", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage errorMsg(LogLevel::ERR, "Service", "Ошибка", "stack trace example");
    REQUIRE(errorMsg.getStackTrace() == "stack trace example");
}

TEST_CASE("LogMessage: INFO без stackTrace", "[LogMessage]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    LogMessage info(LogLevel::INFO, "Auth", "Успешный вход");
    REQUIRE(info.getStackTrace().empty());
}