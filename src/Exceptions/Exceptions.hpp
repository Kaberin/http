#pragma once
#include <exception>
#include <string>

#include "Logger/Logger.hpp"
namespace web {
    namespace exceptions {

        class HTTPException : public std::exception {
           public:
            HTTPException(std::string iMessage) : _msg{iMessage} {}

            const char* what() const noexcept override { return _msg.c_str(); }
            void Print();

           private:
            std::string _msg;
        };

        class ConnectionClosed : public HTTPException {
           public:
            ConnectionClosed(std::string iMessage = "Connection was closed.")
                : HTTPException{iMessage} {}

           private:
        };

        class SocketTimeout : public HTTPException {
           public:
            SocketTimeout(std::string iMessage = "Socket timeout.")
                : HTTPException{iMessage} {}
        };

        class IdleTimeout : public HTTPException {
           public:
            IdleTimeout(std::string iMessage = "Idle timeout.")
                : HTTPException{iMessage} {}
        };

        class SocketError : public HTTPException {
           public:
            SocketError(std::string iMessage = "Socket error.")
                : HTTPException{iMessage} {}
        };
    }  // namespace exceptions
}  // namespace web