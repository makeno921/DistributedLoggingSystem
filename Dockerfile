# ===================== STAGE 1: Builder =====================
FROM ubuntu:24.04 AS builder

# Устанавливаем все зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем весь проект
COPY . .

# Собираем проект
RUN mkdir -p build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release -G Ninja .. && \
    ninja DistributedLoggingSystemTests && \
    ninja scenario_basic \
         scenario_rotation \
         scenario_filtering \
         scenario_stacktrace \
         scenario_multi_client \
         scenario_load_test \
         scenario_json \
         scenario_shutdown \
         scenario_performance

# ===================== STAGE 2: Final Image =====================
FROM ubuntu:24.04

# Минимальные runtime зависимости
RUN apt-get update && apt-get install -y ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем только бинарники
COPY --from=builder /app/build/DistributedLoggingSystemTests .
COPY --from=builder /app/build/scenario_* .

# Создаём директорию для логов
RUN mkdir -p /app/logs && chmod +x /app/*

# По умолчанию запускаем все тесты
ENTRYPOINT ["./DistributedLoggingSystemTests"]
CMD []