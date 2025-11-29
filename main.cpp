#include "server.hpp"
#include "src/Logger/Logger.hpp"
#include "src/Router/Routes.hpp"

int main() {
    web::Logger::GetInstance().Log(web::LogType::INFO,
                                   "The server has started!");
    web::Router router;
    web::RegisterRoutes(router);
    web::Server server(8080, router);
    server.Start();
    return 0;
};
