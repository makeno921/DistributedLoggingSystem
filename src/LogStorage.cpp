#include "LogStorage.h"
#include <iostream>

LogStorage::LogStorage() {
    rotator_ = std::make_unique<FileRotator>();
    std::cout << "[LogStorage] Storage initialized.\n";
}

LogStorage::~LogStorage() = default;

bool LogStorage::save(const LogMessage& msg) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Сохраняем в файл через FileRotator
    bool success = rotator_->write(msg);

    if (success) {
        recentLogs_.push_back(msg);
        // Оставляем только последние 1000 записей в памяти
        if (recentLogs_.size() > 1000) {
            recentLogs_.erase(recentLogs_.begin());
        }
        std::cout << "[LogStorage] Log saved: " << msg.toString() << "\n";
        return true;
    }

    std::cerr << "[LogStorage] Failed to save log!\n";
    return false;
}

std::vector<LogMessage> LogStorage::getRecentLogs(size_t count) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (count >= recentLogs_.size()) {
        return recentLogs_;
    }

    return std::vector<LogMessage>(recentLogs_.end() - count, recentLogs_.end());
}

void LogStorage::cleanupOldLogs(int daysToKeep) {
    std::lock_guard<std::mutex> lock(mutex_);
    rotator_->cleanup(daysToKeep);
    std::cout << "[LogStorage] Old logs cleanup completed (keep " << daysToKeep << " days).\n";
}