add_subdirectory(Exceptions)
add_subdirectory(HTTP)
add_subdirectory(Router)
add_library(Source
    ./ClientHandler.cpp
    ./Utils.cpp
    ./ClientHandler.hpp
    ./Utils.hpp
    ./WebTypes.hpp
    ./DefaulValues.hpp
    ./MagicEnum.hpp
)

target_link_libraries(Source PRIVATE HTTPExceptions)
target_link_libraries(Source PRIVATE HTTP)
target_link_libraries(Source PRIVATE Router)

target_include_directories(Source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})