cmake_minimum_required(VERSION 3.16)
project(WebServerCpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(WebServer
    ./server.cpp
    ./src/Socket.cpp
    ./src/HTTPReader.cpp
    ./src/HTTPParser.cpp
    ./src/ClientHandler.cpp
    ./src/Utils.cpp
    # остальные .cpp
)

add_executable(Tests
    ./tests/main.cpp
)

# Линкуем WinSock
target_link_libraries(WebServer
    ws2_32
)