add_subdirectory(Exceptions)
add_subdirectory(HTTP)
add_subdirectory(Router)
set(SOURCE_FILES
    ${EXCEPTIONS_FILES}
    ${HTTP_FILES}
    ${ROUTER_FILES}
    src/ClientHandler.cpp
    src/Utils.cpp
)
set(SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)