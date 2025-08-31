cmake_minimum_required(VERSION 3.5)

project(kaspersky LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

add_subdirectory(AppConsole)
add_subdirectory(Scaner)
add_subdirectory(Logger)
add_subdirectory(DataBase)

add_executable(kaspersky
    main.cpp
)

target_link_libraries(kaspersky PRIVATE
    AppConsole
    Scaner
    Logger
    DataBase
    ${OPENSSL_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS kaspersky
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
