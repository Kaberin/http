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
    ./src/Socket.hpp
    ./src/HTTPReader.hpp
    ./src/HTTPParser.hpp
    ./src/ClientHandler.hpp
    ./src/Utils.hpp
    ./src/WebTypes.hpp
)

set_target_properties(WebServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(Tests
    ./tests/main.cpp
)

if(WIN32)
    target_link_libraries(WebServer
        ws2_32
    )
endif()