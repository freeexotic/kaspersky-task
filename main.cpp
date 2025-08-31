#include <iostream>
#include <string>

#include "AppConsole/appconsole.hpp"

std::string base_url;
std::string log_url;
std::string path_url;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Недостаточное кол-во аргументов | неправильный запуск программы.\n";
        std::cerr << "Формат запуска: ./start --base base.csv --log report.log --path /path/to/folder\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string argument = argv[i];
        if (argument == "--base" || argument == "--b") {
            if (i + 1 < argc) {
                base_url = argv[++i];
                std::cout << "Адрес базы данных хешей вирусов: " << base_url << std::endl;
            } else {
                std::cerr << "Ошибка: для ключа " << argument << " требуется значение." << std::endl;
                return 1;
            }
        } else if (argument == "--log" || argument == "--l") {
            if (i + 1 < argc) {
                log_url = argv[++i];
                std::cout << "Адрес файла логирования: " << log_url << std::endl;
            } else {
                std::cerr << "Ошибка: для ключа " << argument << " требуется значение." << std::endl;
                return 1;
            }
        } else if (argument == "--path" || argument == "--p") {
            if (i + 1 < argc) {
                path_url = argv[++i];
                std::cout << "Адрес папки для анализа: " << path_url << std::endl;
            } else {
                std::cerr << "Ошибка: для ключа " << argument << " требуется значение." << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Неизвестный параметр: " << argument << std::endl;
            return 1;
        }
    }

    if (base_url.empty() || log_url.empty() || path_url.empty()) {
        std::cerr << "Ошибка: все параметры (--base, --log, --path) должны быть указаны." << std::endl;
        return 1;
    }

    std::cout << "Параметры получены успешно!" << std::endl;

    AppConsole app(std::move(base_url), std::move(log_url), std::move(path_url));

    return 0;
}
