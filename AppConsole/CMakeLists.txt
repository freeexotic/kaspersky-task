add_library(AppConsole STATIC
    appconsole.cpp
)

# Указываем, где находятся заголовки зависимостей
target_include_directories(AppConsole PRIVATE
    ../Logger
    ../Scaner
    ../DataBase
)

# Также делаем заголовки AppConsole доступными
target_include_directories(AppConsole PUBLIC .)
