#pragma once

#include <string>
#include <unordered_map>
#include "rapidcsv.h"

class DataBase
{
public:
    DataBase(const std::string& path);

    std::string Verdict(const std::string& hash);

    std::unordered_map<std::string, std::string> hash_map;

};

