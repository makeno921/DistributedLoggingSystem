# Система логирования для распределённых приложений

## Общая информация

**Тема:** Система логирования для распределённых приложений (клиент-серверная архитектура)  
**Язык:** C++20  
**Сборка:** CMake + Ninja  
**Тестирование:** Catch2  
**Контейнеризация:** Docker

---

## Структура проекта

```
DistributedLoggingSystem/
├── src/                    # Основной код
│   ├── LogMessage.h/cpp    # Структура лога (уровни, JSON, stackTrace)
│   ├── LoggerClient.h/cpp  # Клиент отправки логов по TCP
│   ├── LogServer.h/cpp     # Сервер агрегации логов (многопоточный)
│   ├── LogStorage.h/cpp    # Хранилище логов в памяти
│   ├── FileRotator.h/cpp   # Ротация файлов по дате и размеру
│   ├── TcpConnection.h/cpp # TCP-соединение (POSIX сокеты)
│   └── main.cpp
├── tests/                  # Unit-тесты Catch2 (39 тестов)
│   └── scenarios/          # 9 интеграционных сценариев
├── logs/                   # Директория для логов
├── CMakeLists.txt
├── Dockerfile
├── .dockerignore
└── README.md
```

---

## Практическое занятие №2 — Базовая архитектура

**Выполнено:**

- Спроектирована клиент-серверная архитектура
- Реализован класс `LogMessage` (уровни логирования, timestamp, requestId, toString, toJson, stackTrace)
- Реализован `FileRotator` с ротацией по размеру файла
- Реализован `LogStorage` для агрегации и сохранения логов
- Реализованы базовые `LoggerClient` и `LogServer`
- Настроена сборка проекта через CMake

**Результат:** Базовая функциональность системы логирования готова.

---

## Практическое занятие №3 — Паттерны + Системы сборки

**Выполнено:**

- Реализовано реальное TCP-соединение (`TcpConnection`)
- Реализована многопоточная обработка клиентов на сервере
- Клиент отправляет логи по сети в формате JSON
- Сервер принимает, парсит JSON и сохраняет логи
- Добавлена корректная обработка подключений/отключений
- Применены паттерны: Singleton, Factory Method, Observer, Facade, RAII

**Результат:** Полноценная распределённая клиент-серверная система логирования.

---

## Практическое занятие №4 — Тестирование

### Unit-тесты (Catch2)

- **39 тестов**, **93 assertions**
- Полностью покрыты классы: `LogMessage`, `FileRotator`, `LogStorage`, `LogServer`, `LoggerClient`, `TcpConnection`

### Тестовые сценарии (9 штук)

1. `scenario_basic` — Базовый сценарий работы системы
2. `scenario_rotation` — Ротация логов по размеру
3. `scenario_filtering` — Фильтрация в реальном времени
4. `scenario_stacktrace` — Обработка stackTrace
5. `scenario_multi_client` — Работа с несколькими клиентами
6. `scenario_load_test` — Тестирование разных сервисов под нагрузкой
7. `scenario_json` — Тестирование JSON-формата
8. `scenario_shutdown` — Устойчивость к отключениям
9. `scenario_performance` — Производительность при большой нагрузке

Все сценарии реализованы как отдельные исполняемые программы.

---

## Практическое занятие №5 — Контейнеризация (Docker)

**Выполнено:**

- Подготовлен многостадийный `Dockerfile` (builder + runtime)
- Проект собирается и тестируется в **чистом окружении Linux** (Ubuntu 24.04)
- Все зависимости явно установлены в Dockerfile
- Запуск unit-тестов Catch2 внутри контейнера
- Организована передача аргументов командной строки
- Docker-образ не содержит временных файлов сборки

---

## Сборка и запуск

### Без Docker (локально, macOS / Linux)

**Требования:** `cmake`, `ninja`, `g++` (или `clang++`), `git`

```bash
# 1. Клонировать репозиторий
git clone https://github.com/makeno921/DistributedLoggingSystem.git
cd DistributedLoggingSystem

# 2. Собрать проект
cmake -S . -B build -G Ninja
cmake --build build

# 3. Запустить основное приложение
cd build
./DistributedLoggingSystem

# 4. Запустить все unit-тесты
./DistributedLoggingSystemTests

# 5. Запустить конкретный сценарий
./scenario_basic
./scenario_json
./scenario_performance
```

---

### С Docker (чистое окружение Linux — Ubuntu 24.04)

**Требования:** установленный [Docker Desktop](https://www.docker.com/products/docker-desktop/)

#### Сборка образа

```bash
docker build -t dls-logging .
```

#### Запуск всех unit-тестов Catch2 внутри контейнера

```bash
docker run --rm dls-logging
```

#### Запуск тестов с сохранением логов на хост-машину

```bash
# macOS / Linux
docker run --rm -v "$(pwd)/docker_logs:/app/logs" dls-logging

# Windows
docker run --rm -v "%CD%\docker_logs:/app/logs" dls-logging
```

#### Запуск каждого сценария в чистом Linux окружении

```bash
# Сценарий 1: Базовый сценарий работы системы
docker run --rm --entrypoint ./scenario_basic dls-logging

# Сценарий 2: Ротация логов по размеру
docker run --rm --entrypoint ./scenario_rotation dls-logging

# Сценарий 3: Фильтрация в реальном времени
docker run --rm --entrypoint ./scenario_filtering dls-logging

# Сценарий 4: Обработка stackTrace
docker run --rm --entrypoint ./scenario_stacktrace dls-logging

# Сценарий 5: Работа с несколькими клиентами
docker run --rm --entrypoint ./scenario_multi_client dls-logging

# Сценарий 6: Тестирование под нагрузкой
docker run --rm --entrypoint ./scenario_load_test dls-logging

# Сценарий 7: Тестирование JSON-формата
docker run --rm --entrypoint ./scenario_json dls-logging

# Сценарий 8: Устойчивость к отключениям
docker run --rm --entrypoint ./scenario_shutdown dls-logging

# Сценарий 9: Производительность при большой нагрузке
docker run --rm --entrypoint ./scenario_performance dls-logging
```

---

## Используемые технологии и библиотеки

| Технология | Назначение |
|------------|------------|
| C++20 | Основной язык разработки |
| CMake 3.15+ | Система сборки |
| Ninja | Бэкенд сборки |
| Catch2 | Фреймворк для unit-тестирования |
| POSIX Sockets | TCP-соединение (кросс-платформенно) |
| Docker / Ubuntu 24.04 | Контейнеризация и чистое Linux окружение |

---

## Автор

**Максимов Святослав Эдуардович**  
Группа: ЭФБО-0X-24  
Дисциплина: Программирование корпоративных систем  
Преподаватель: Сокунов Дмитрий Антонович
