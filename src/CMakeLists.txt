add_subdirectory(Exceptions)
add_subdirectory(HTTP)
add_subdirectory(Router)
set(SOURCE_FILES
    ${EXCEPTIONS_FILES}
    ${HTTP_FILES}
    ${ROUTER_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/ClientHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DefaulValues.hpp
)
set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)