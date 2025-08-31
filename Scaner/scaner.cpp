#include "scaner.hpp"
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include <iomanip>

Scaner::Scaner() {}


std::string Scaner::FileScaner(const std::string& file_path){
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    char buffer[1024];
    unsigned char hash[EVP_MAX_MD_SIZE]; // Максимальный размер хеша
    unsigned int hash_length;

    // Создаем контекст для вычисления хеша
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP context");
    }

    // Инициализируем контекст для MD5
    if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize MD5 context");
    }

    // Читаем файл блоками и обновляем хеш
    while (file.good()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize bytes_read = file.gcount();
        if (bytes_read > 0) {
            if (EVP_DigestUpdate(ctx, buffer, bytes_read) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("Failed to update MD5 digest");
            }
        }
    }

    // Завершаем вычисление хеша
    if (EVP_DigestFinal_ex(ctx, hash, &hash_length) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize MD5 digest");
    }

    // Освобождаем контекст
    EVP_MD_CTX_free(ctx);

    // Преобразуем хеш в шестнадцатеричную строку
    std::ostringstream result;
    for (unsigned int i = 0; i < hash_length; ++i) {
        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    //Разобрать

    return result.str();
}


// !!!!!!!!!!!!!!!!!!!
