#Builder
FROM ubuntu:24.04 AS builder

#зависимости
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

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

# Image
FROM ubuntu:24.04

# Минимальные зависимости
RUN apt-get update && apt-get install -y ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

# онли копия бинарников
COPY --from=builder /app/build/DistributedLoggingSystemTests .
COPY --from=builder /app/build/scenario_* .

# директория для логов
RUN mkdir -p /app/logs && chmod +x /app/*

# по дефолту запуск всех тестов
ENTRYPOINT ["./DistributedLoggingSystemTests"]
CMD []