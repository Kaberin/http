add_library(HTTP
    HTTPParser.cpp
    HTTPParser.hpp
    HTTPReader.cpp
    HTTPReader.hpp
    Socket.hpp
    Socket.cpp
)

target_include_directories(HTTP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})