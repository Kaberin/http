//Server.cpp
#include <iostream>
#include <thread>

#include "src/Utils.hpp"
#include "src/Socket.hpp"
#include "src/HTTPReader.hpp"
#include "src/ClientHandler.hpp"
int main()
{
    std::cout << "Server is running!\n";
    web::WSAInit init;
    web::Socket serverSocket(8080);

    while (true) {
        auto clientSocket = serverSocket.AcceptConnection();
        auto req = clientSocket.value().GetHTTPRequest();
        std::cout << req << '\n';
        web::ClientHandler clientHandler(std::move(clientSocket.value()), req);
        std::thread clientThread{ std::move(clientHandler) };
        clientThread.detach();
    }
    return 0;
}