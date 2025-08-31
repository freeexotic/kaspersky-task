#include "rapidcsv.h"
#include "database.hpp"
#include <iostream>
#include <filesystem>

DataBase::DataBase(const std::string& path) {
    try {
        // Проверяем существование файла
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("File not found: " + path);
        }

        // Открываем CSV-файл с явным указанием разделителя (;)
        rapidcsv::Document HashBase(
            path,
            rapidcsv::LabelParams(-1, -1), // Без заголовков
            rapidcsv::SeparatorParams(';') // Разделитель ";"
            );

        size_t rowCount = HashBase.GetRowCount();
        size_t columnCount = HashBase.GetColumnCount();

        // Проверяем, что файл содержит хотя бы два столбца
        if (columnCount < 2) {
            throw std::runtime_error("CSV file must contain at least two columns.");
        }

        std::cout << "Processing CSV file with " << rowCount << " rows..." << std::endl;

        for (size_t i = 0; i < rowCount; ++i) {
            try {
                // Получаем строку
                std::vector<std::string> row = HashBase.GetRow<std::string>(i);

                // Логируем содержимое строки
                std::cout << "Row " << i << ": ";
                for (const auto& cell : row) {
                    std::cout << "[" << cell << "] ";
                }
                std::cout << std::endl;

                // Пропускаем строки с недостаточным количеством данных
                if (row.size() < 2 || row[0].empty() || row[1].empty()) {
                    std::cerr << "Skipping invalid row at index " << i << ": insufficient data." << std::endl;
                    continue;
                }

                // Сохраняем хеш и вердикт
                std::string hash_b = row[0];
                std::string verdict_b = row[1];

                hash_map[hash_b] = verdict_b;
            } catch (const std::exception& e) {
                std::cerr << "Error processing row " << i << ": " << e.what() << std::endl;
                continue;
            }
        }

        std::cout << "Database loaded successfully." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Вывод загруженных данных
    for (const auto& pair : hash_map) {
        std::cout << "Hash: " << pair.first << ", Verdict: " << pair.second << std::endl;
    }
}

std::string DataBase::Verdict(const std::string& hash) {
    if (hash_map.count(hash) > 0) {
        return hash_map[hash];
    } else {
        return "Clear";
    }
}
