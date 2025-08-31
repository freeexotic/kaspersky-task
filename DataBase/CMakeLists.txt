# DataBase/CMakeLists.txt
add_library(DataBase STATIC
    database.cpp
)

target_include_directories(DataBase PUBLIC .)
