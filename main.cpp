#include "server.hpp"
#include "src/Router/Routes.hpp"

int main() {
#ifdef _WIN32
    web::WSAInit init;
#endif
    web::Router router;
    web::RegisterRoutes(router);
    web::Server server(8080, router);
    server.Start();
    return 0;
};
