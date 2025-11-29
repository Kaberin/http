#pragma once

#include "./HTTP/Socket.hpp"
#include "./Router/Router.hpp"
#include "DefaulValues.hpp"
#include "Exceptions/Exceptions.hpp"
#include "Logger/Logger.hpp"
#include "WebTypes.hpp"
namespace web {
    class ClientHandler {
       public:
        ClientHandler(Socket&& iClientSocket)
            : _socket{std::move(iClientSocket)} {}

        void operator()(const Router& iRouter);

       private:
        void ExceptionHandler(std::exception& e)  // ONLY TEMPORARY
        {
            Logger::GetInstance().Log(LogType::ERROR, e.what());
            HTTPResponse response{
                HTTPVersion::HTTP1_1,
                StatusCode::Timeout,
                {
                    {HeaderNames::CONNECTION, HeaderValues::CONNECTION_CLOSE},
                    {HeaderNames::CONTENT_TYPE,
                     HeaderValues::CONTENT_TYPE_TEXT_PLAIN},
                },
                e.what()};
            _socket.Send(response.ToString());
        }

        Socket _socket;
    };
}  // namespace web