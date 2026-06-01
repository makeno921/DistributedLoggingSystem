#pragma once
#include "LogMessage.h"
#include "FileRotator.h"
#include <vector>
#include <memory>
#include <mutex>

class LogStorage {
public:
    LogStorage();
    ~LogStorage();

    // Сохранение лога
    bool save(const LogMessage& msg);

    std::vector<LogMessage> getRecentLogs(size_t count = 100);

    // Очистка старых логов (по дате)
    void cleanupOldLogs(int daysToKeep = 7);

private:
    std::unique_ptr<FileRotator> rotator_;
    std::vector<LogMessage> recentLogs_;        // кэш последних логов в памяти
    mutable std::mutex mutex_;                   // потокобезопасность
};