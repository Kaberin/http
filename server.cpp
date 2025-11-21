//Server.cpp
#include <thread>
#include "Server.hpp"

namespace web {
    Server::Server(int iPort, Router iRouter) : _port{ iPort }, _router{ iRouter } {}

    void Server::Start()
    {
        web::Socket serverSocket(_port, web::SocketType::Server, SOMAXCONN);
        while (true) {
            auto clientSocketOpt = serverSocket.AcceptConnection();
            if (!clientSocketOpt) {
                continue;
            }

            std::thread clientThread([socket = std::move(*clientSocketOpt), this]() mutable {
                web::ClientHandler handler(std::move(socket));
                handler(std::cref(_router));
                });
            clientThread.detach();
        }
    }
}
