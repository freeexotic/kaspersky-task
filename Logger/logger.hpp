#pragma once

#include <string>
#include <fstream>
#include <mutex>

class Logger {
public:
    Logger(const std::string& log_path);
    ~Logger();

    void Log(const std::string& file_path, const std::string& hash, const std::string& verdict);

private:
    std::ofstream log_file;
    std::mutex log_mutex;
};

