# Logger/CMakeLists.txt
add_library(Logger STATIC
    logger.cpp
)

target_include_directories(Logger PUBLIC .)
