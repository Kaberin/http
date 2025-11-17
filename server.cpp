//Server.cpp
#include <iostream>
#include <thread>

#include "src/Utils.hpp"
#include "src/Socket.hpp"
#include "src/HTTPReader.hpp"
#include "src/ClientHandler.hpp"
#include "src/WebTypes.hpp"


int main()
{
    //web::HTTPResponse response{
    //    web::HTTPVersion::HTTP1_1,
    //    web::StatusCode::Ok,
    //    {
    //        {"Server", "San-Francisco"},
    //        {"Content-Type", "text/plain"},
    //        {"Connection", "keep-alive"}
    //    },
    //    "Hello world!"
    //};

    //std::cout << response.ToString() << '\n';
    std::cout << "Server is running!\n";
#ifdef _WIN32
    web::WSAInit init;
#endif
    web::Socket serverSocket(8080, web::SocketType::Server, SOMAXCONN);

    while (true) {
        auto clientSocketOpt = serverSocket.AcceptConnection();
        if (!clientSocketOpt) {
            continue;
        }

        std::thread clientThread([socket = std::move(*clientSocketOpt)]() mutable {
            web::ClientHandler handler(std::move(socket));
            handler();
            });
        clientThread.detach();
    }
    return 0;
}