#pragma once
#include "LogMessage.h"
#include <fstream>
#include <string>
#include <mutex>

class FileRotator {
public:
    explicit FileRotator(const std::string& baseDir = "logs");
    ~FileRotator();

    bool write(const LogMessage& msg);

    void cleanup(int daysToKeep);   // <-- добавил обратно

private:
    void rotate();
    bool needsRotation() const;
    std::string getCurrentLogFileName() const;
    void openNewFile();
    void closeCurrentFile();

    std::ofstream currentFile_;
    std::string currentFilePath_;
    std::string baseDir_;
    mutable std::mutex mutex_;
    int rotationCounter_ = 0;
    static constexpr size_t MAX_FILE_SIZE = 15 * 1024; // 100 КБ
};