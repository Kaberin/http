#pragma once

#include "./HTTP/Socket.hpp"
#include "WebTypes.hpp"
namespace web {
    class ClientHandler {
    public:
        ClientHandler(Socket&& iClientSocket) : _socket{ std::move(iClientSocket) }
        {
        }

        void operator()();
    private:
        Socket _socket;
    };
}