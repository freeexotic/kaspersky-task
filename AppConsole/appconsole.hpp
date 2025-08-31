#pragma once

#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include "logger.hpp"
#include "database.hpp"
#include "scaner.hpp"

class AppConsole {
public:
    AppConsole(std::string &&base, std::string &&log, std::string &&path);

    bool Scan(const std::string& path);
    void PrintReport();

    struct Statistics {
        std::atomic<int> errors = 0;
        std::atomic<int> files = 0;
        std::atomic<int> virus_files = 0;
        std::atomic<int> processed_files = 0;
        std::chrono::steady_clock::time_point startTime;
        std::chrono::steady_clock::time_point endTime;
    };

    std::string base_url;
    std::string log_url;
    std::string path_url;

private:
    Logger logger;
    DataBase database;
    Scaner scaner;
    Statistics stats;
    
    size_t thread_count;
    
    struct FileTask {
        std::string file_path;
    };
    
    std::queue<FileTask> task_queue;
    std::mutex queue_mutex;
    std::condition_variable queue_cv;
    std::atomic<bool> scanning_finished{false};
    
    void WorkerThread();
    void ProcessFile(const std::string& file_path);
    void CollectFiles(const std::string& path);
};

