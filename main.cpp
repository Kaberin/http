#include "server.hpp"

web::HTTPResponse handler1(const web::HTTPRequest& req)
{
    std::cout << req << '\n';

    web::HTTPResponse resp;
    resp._version = web::HTTPVersion::HTTP1_1;
    resp._statusCode = web::StatusCode::Ok;
    if (req._headers.at("Connection") == "close") {
        resp._headers = {
            {"Connection", "close"}
        };
    }
    else {
        resp._headers = {
            {"Connection", "keep-alive"}
        };
    }
    resp._body = "Message from client: " + req._body;
    return resp;
}

int main() {
#ifdef _WIN32
    web::WSAInit init;
#endif
    web::Router router;
    router.AddRoute("api/key/value", web::HTTPMethod::GET, handler1);
    web::Server server(8080, router);
    server.Start();
    return 0;
};
