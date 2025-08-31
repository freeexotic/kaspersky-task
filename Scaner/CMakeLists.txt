# Scaner/CMakeLists.txt
add_library(Scaner STATIC
    scaner.cpp
)

# Добавляем путь к заголовкам AppConsole и Logger
target_include_directories(Scaner PRIVATE
    ../AppConsole
    ../Logger
)

# Связываем с OpenSSL
target_link_libraries(Scaner PRIVATE ${OPENSSL_LIBRARIES})

target_include_directories(AppConsole PUBLIC .)
