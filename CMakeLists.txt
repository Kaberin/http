cmake_minimum_required(VERSION 3.16)
project(WebServerCpp)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(WebServer
    ${SOURCE_FILES}
    server.cpp
    main.cpp
)

if(WIN32)
    target_link_libraries(WebServer PRIVATE ws2_32)
endif()

set_target_properties(WebServer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES server.cpp main.cpp)