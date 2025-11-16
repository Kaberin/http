add_library(srcLib
    ./Socket.cpp
    ./HTTPReader.cpp
    ./HTTPParser.cpp
    ./ClientHandler.cpp
    ./Utils.cpp
    ./Socket.hpp
    ./HTTPReader.hpp
    ./HTTPParser.hpp
    ./ClientHandler.hpp
    ./Utils.hpp
    ./WebTypes.hpp
)

target_include_directories(srcLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})