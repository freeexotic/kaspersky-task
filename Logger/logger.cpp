#include "logger.hpp"
#include <iostream>

Logger::Logger(const std::string& log_path) {
    log_file.open(log_path, std::ios::out | std::ios::app);
    if (!log_file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл лога: " + log_path);
    }
}

Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

void Logger::Log(const std::string& file_path, const std::string& hash, const std::string& verdict) {
    std::lock_guard<std::mutex> lock(log_mutex);
    if (log_file.is_open()) {
        log_file << "Файл: " << file_path << "\n";
        log_file << "Хеш: " << hash << "\n";
        log_file << "Вердикт: " << verdict << "\n";
        log_file << "-------------------------\n";
        log_file.flush();
    } else {
        std::cerr << "Ошибка: лог-файл не открыт." << std::endl;
    }
}
