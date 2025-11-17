add_library(HTTPExceptions
    ./Exceptions.cpp
    ./Exceptions.hpp
)

target_include_directories(HTTPExceptions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})