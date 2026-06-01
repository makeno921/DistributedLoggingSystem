#include "FileRotator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <filesystem>

FileRotator::FileRotator(const std::string& baseDir) {
    std::filesystem::path projectRoot = std::filesystem::current_path();

    if (projectRoot.filename() == "cmake-build-debug" ||
        projectRoot.filename() == "build" ||
        projectRoot.filename() == "cmake-build-release") {
        projectRoot = projectRoot.parent_path();
    }

    baseDir_ = (projectRoot / baseDir).string();

    std::filesystem::create_directories(baseDir_);
    openNewFile();

    std::cout << "[FileRotator] Логи сохраняются в: " << baseDir_ << std::endl;
    std::cout << "[FileRotator] Лимит ротации по размеру: 15 КБ\n";
}

FileRotator::~FileRotator() {
    closeCurrentFile();
}

bool FileRotator::write(const LogMessage& msg) {
    std::lock_guard<std::mutex> lock(mutex_);

    if (needsRotation()) {
        rotate();
    }

    if (!currentFile_.is_open()) {
        openNewFile();
    }

    currentFile_ << msg.toJson() << std::endl;
    currentFile_.flush();

    return true;
}

void FileRotator::rotate() {
    closeCurrentFile();
    rotationCounter_++;
    openNewFile();
    std::cout << "[FileRotator] 🔄 Ротация по размеру: создан новый файл\n";
}

bool FileRotator::needsRotation() const {
    if (!currentFile_.is_open()) return true;

    try {
        uintmax_t currentSize = std::filesystem::file_size(currentFilePath_);
        if (currentSize > MAX_FILE_SIZE) {
            std::cout << "[FileRotator] Файл достиг " << (currentSize / 1024)
                      << " КБ → ротация\n";
            return true;
        }
    } catch (...) {}
    return false;
}

std::string FileRotator::getCurrentLogFileName() const {
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << "logs_" << std::put_time(std::localtime(&timeT), "%Y-%m-%d");

    if (rotationCounter_ > 0) {
        ss << "_" << std::setfill('0') << std::setw(3) << rotationCounter_;
    }
    ss << ".log";

    return baseDir_ + "/" + ss.str();
}

void FileRotator::openNewFile() {
    closeCurrentFile();
    currentFilePath_ = getCurrentLogFileName();
    currentFile_.open(currentFilePath_, std::ios::app);
    std::cout << "[FileRotator] Новый файл: " << currentFilePath_ << std::endl;
}

void FileRotator::closeCurrentFile() {
    if (currentFile_.is_open()) {
        currentFile_.close();
    }
}

void FileRotator::cleanup(int daysToKeep) {
    std::cout << "[FileRotator] Очистка старых логов...\n";
}