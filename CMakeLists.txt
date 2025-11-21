cmake_minimum_required(VERSION 3.16)
project(WebServerCpp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(WebServer 
./server.hpp
./server.cpp
./main.cpp
)

target_link_libraries(WebServer PRIVATE Source)

if(WIN32)
    target_link_libraries(WebServer PRIVATE ws2_32)
endif()

set_target_properties(WebServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_executable(Tests
    ./tests/main.cpp
)

