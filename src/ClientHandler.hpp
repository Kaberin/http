#pragma once
#include "Socket.hpp"
#include "WebTypes.hpp"
namespace web {
    class ClientHandler {
    public:
        ClientHandler(Socket&& iClientSocket, HTTPRequest request) : _request{ request }, _socket{ std::move(iClientSocket) } {
            std::cout << "Client Handler constructor\n";
        }

        void operator()();
    private:
        Socket _socket;
        HTTPRequest _request;
    };
}