cmake_minimum_required(VERSION 3.14)
project(DistributedLoggingSystem)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тестирование
enable_testing()

# Подключаем Catch2
include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

# === ОБЩИЕ ИСХОДНИКИ (используются и в программе, и в тестах) ===
set(COMMON_SOURCES
        src/LogMessage.cpp
        src/LoggerClient.cpp
        src/LogServer.cpp
        src/LogStorage.cpp
        src/FileRotator.cpp
        src/TcpConnection.cpp
)

# Создаём библиотеку из общих файлов
add_library(DistributedLoggingLib STATIC ${COMMON_SOURCES})
target_include_directories(DistributedLoggingLib PUBLIC include src)

if(WIN32)
    target_link_libraries(DistributedLoggingLib PRIVATE ws2_32)
endif()

# === ОСНОВНАЯ ПРОГРАММА ===
add_executable(DistributedLoggingSystem src/main.cpp)
target_link_libraries(DistributedLoggingSystem PRIVATE DistributedLoggingLib)

# === ТЕСТЫ ===
add_executable(DistributedLoggingSystemTests
        tests/test_LogMessage.cpp
        tests/test_FileRotator.cpp
        tests/test_LogStorage.cpp
        tests/test_common.h
        tests/test_LogServer.cpp
        tests/test_TcpConnection.cpp
        tests/test_LoggerClient.cpp
)

target_link_libraries(DistributedLoggingSystemTests PRIVATE
        DistributedLoggingLib
        Catch2::Catch2WithMain
)

# Регистрируем тесты
include(CTest)
include(Catch)
catch_discover_tests(DistributedLoggingSystemTests)

message(STATUS "=== Distributed Logging System + Tests ===")
message(STATUS "Main target: DistributedLoggingSystem")
message(STATUS "Tests target: DistributedLoggingSystemTests")

# ====================== СЦЕНАРИИ ДЛЯ 4 ПРАКТИКИ ======================
add_executable(scenario_basic tests/scenarios/scenario_basic.cpp)
target_link_libraries(scenario_basic PRIVATE DistributedLoggingLib)

add_executable(scenario_rotation tests/scenarios/scenario_rotation.cpp)
target_link_libraries(scenario_rotation PRIVATE DistributedLoggingLib)

add_executable(scenario_filtering tests/scenarios/scenario_filtering.cpp)
target_link_libraries(scenario_filtering PRIVATE DistributedLoggingLib)

add_executable(scenario_stacktrace tests/scenarios/scenario_stacktrace.cpp)
target_link_libraries(scenario_stacktrace PRIVATE DistributedLoggingLib)

add_executable(scenario_multi_client tests/scenarios/scenario_multi_client.cpp)
target_link_libraries(scenario_multi_client PRIVATE DistributedLoggingLib)

add_executable(scenario_load_test tests/scenarios/scenario_load_test.cpp)
target_link_libraries(scenario_load_test PRIVATE DistributedLoggingLib)

add_executable(scenario_json tests/scenarios/scenario_json.cpp)
target_link_libraries(scenario_json PRIVATE DistributedLoggingLib)

add_executable(scenario_shutdown tests/scenarios/scenario_shutdown.cpp)
target_link_libraries(scenario_shutdown PRIVATE DistributedLoggingLib)

add_executable(scenario_performance tests/scenarios/scenario_performance.cpp)
target_link_libraries(scenario_performance PRIVATE DistributedLoggingLib)
