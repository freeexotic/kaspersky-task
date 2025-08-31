#include <iostream>
#include <filesystem>
#include <iomanip>
#include "appconsole.hpp"

AppConsole::AppConsole(std::string &&b_url, std::string &&l_url, std::string &&p_url)
    : base_url(b_url), log_url(l_url), path_url(p_url), database(b_url), logger(l_url) {
    
    unsigned int hw_threads = std::thread::hardware_concurrency();
    thread_count = hw_threads ? hw_threads : 4;
    std::cout << "База данных: " << base_url << std::endl;
    std::cout << "Лог-файл: " << log_url << std::endl;
    std::cout << "Путь для сканирования: " << path_url << std::endl;
    std::cout << "Количество потоков: " << thread_count << std::endl;

    stats.startTime = std::chrono::steady_clock::now();
    this->Scan(path_url);
    stats.endTime = std::chrono::steady_clock::now();

    PrintReport();
}

bool AppConsole::Scan(const std::string& path) {
    try {
        if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
            std::cerr << "Ошибка: указанный путь не является директорией." << std::endl;
            return false;
        }

        std::cout << "Собираем список файлов..." << std::endl;
        CollectFiles(path);
        std::cout << "Найдено файлов для сканирования: " << stats.files << std::endl;

        if (stats.files == 0) {
            std::cout << "Нет файлов для сканирования." << std::endl;
            return true;
        }

        std::cout << "Запускаем сканирование на " << thread_count << " потоках..." << std::endl;

        std::vector<std::thread> workers;
        for (size_t i = 0; i < thread_count; ++i) {
            workers.emplace_back(&AppConsole::WorkerThread, this);
        }

        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }

        std::cout << "Сканирование завершено." << std::endl;
        return true;

    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Ошибка файловой системы: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return false;
    }
}

void AppConsole::PrintReport() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stats.endTime - stats.startTime).count();
    std::cout << "\n=== Отчет о сканировании ===\n";
    std::cout << "Общее количество файлов: " << stats.files << "\n";
    std::cout << "Обработано файлов: " << stats.processed_files << "\n";
    std::cout << "Количество вредоносных файлов: " << stats.virus_files << "\n";
    std::cout << "Количество ошибок: " << stats.errors << "\n";
    std::cout << "Время выполнения: " << duration << " мс\n";
    std::cout << "Количество потоков: " << thread_count << "\n";
    std::cout << "=============================\n";
}

void AppConsole::CollectFiles(const std::string& path) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (std::filesystem::is_regular_file(entry)) {
                ++stats.files;
                FileTask task;
                task.file_path = entry.path().string();
                
                std::lock_guard<std::mutex> lock(queue_mutex);
                task_queue.push(task);
            }
        }
        
        scanning_finished = true;
        queue_cv.notify_all();
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сборе файлов: " << e.what() << std::endl;
        scanning_finished = true;
        queue_cv.notify_all();
    }
}

void AppConsole::WorkerThread() {
    while (true) {
        FileTask task;
        bool has_task = false;
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [this] { 
                return !task_queue.empty() || scanning_finished; 
            });
            
            if (!task_queue.empty()) {
                task = task_queue.front();
                task_queue.pop();
                has_task = true;
            }
        }
        
        if (has_task) {
            ProcessFile(task.file_path);
        } else if (scanning_finished) {
            break;
        }
    }
}

void AppConsole::ProcessFile(const std::string& file_path) {
    try {
        std::string file_hash = scaner.FileScaner(file_path);
        std::string verdict = database.Verdict(file_hash);

        if (verdict != "Clear") {
            ++stats.virus_files;
            logger.Log(file_path, file_hash, verdict);
        }

        ++stats.processed_files;
        
        static std::mutex progress_mutex;
        std::lock_guard<std::mutex> progress_lock(progress_mutex);
        
        if (stats.processed_files % 10 == 0 || verdict != "Clear") {
            double progress = (double)stats.processed_files / stats.files * 100.0;
            std::cout << std::fixed << std::setprecision(1) 
                      << "[" << progress << "%] Обработано: " << stats.processed_files 
                      << "/" << stats.files;
            if (verdict != "Clear") {
                std::cout << " | УГРОЗА: " << file_path << " (" << verdict << ")";
            }
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        ++stats.errors;
        std::cerr << "Ошибка при обработке файла " << file_path << ": " << e.what() << std::endl;
    }
}

