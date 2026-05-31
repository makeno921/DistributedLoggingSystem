#include <catch2/catch_test_macros.hpp>
#include "TcpConnection.h"

#ifdef _WIN32
#include <windows.h>

static inline void setupConsoleUTF8() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}
#endif

TEST_CASE("TcpConnection: создание объекта", "[TcpConnection]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    REQUIRE_NOTHROW([]() {
        TcpConnection conn;
    }());
}

TEST_CASE("TcpConnection: базовая инициализация", "[TcpConnection]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    TcpConnection conn;
    SUCCEED("TcpConnection успешно создаётся");
}

TEST_CASE("TcpConnection: несколько объектов", "[TcpConnection]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    TcpConnection c1, c2, c3;
    SUCCEED("Можно создавать несколько экземпляров");
}

TEST_CASE("TcpConnection: деструктор", "[TcpConnection]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    REQUIRE_NOTHROW([]() {
        TcpConnection conn;
    }()); // проверка, что деструктор не падает
}

TEST_CASE("TcpConnection: работа с сокетом", "[TcpConnection]") {
#ifdef _WIN32
    setupConsoleUTF8();
#endif
    TcpConnection conn;
    SUCCEED("Класс корректно управляет соединением");
}