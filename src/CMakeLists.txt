add_subdirectory(Exceptions)
add_subdirectory(HTTP)
add_library(srcLib
    ./ClientHandler.cpp
    ./Utils.cpp
    ./ClientHandler.hpp
    ./Utils.hpp
    ./WebTypes.hpp
    ./DefaulValues.hpp
    ./MagicEnum.hpp
)

target_link_libraries(srcLib PRIVATE HTTPExceptions)
target_link_libraries(srcLib PRIVATE HTTP)

target_include_directories(srcLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})