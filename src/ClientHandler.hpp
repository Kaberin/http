#pragma once

#include "./HTTP/Socket.hpp"
#include "WebTypes.hpp"
#include "Exceptions/Exceptions.hpp"
#include "./Router/Router.hpp"
namespace web {
    class ClientHandler {
    public:
        ClientHandler(Socket&& iClientSocket) : _socket{ std::move(iClientSocket) }
        {
        }

        void operator()(const Router& iRouter);
    private:

        void ExceptionHandler(std::exception& e) //ONLY TEMPORARY
        {
            std::cout << e.what() << "\n" << "Closing connection\n" << '\n';
            HTTPResponse response{
                   HTTPVersion::HTTP1_1,
                   StatusCode::Timeout,
                   {
                       {"Connection", "close"},
                       {"Content-Type", "text/plain"},
                   },
                   e.what()
            };
            _socket.Send(response.ToString());
        }

        Socket _socket;
    };
}