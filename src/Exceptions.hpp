#pragma once
#include <exception>
namespace web {
    namespace exceptions {
        class ConnectionClosed : public std::exception {
        public:
            ConnectionClosed(const char* Message = "Connection was closed.") :std::exception{Message}{}
        };

        class Timeout : public std::exception {
        public:
            Timeout(const char* Message = "Server timeout.") :std::exception{ Message }{}
        };

        class SocketError : public std::exception {
        public:
            SocketError(const char* Message = "Socket error.") :std::exception{ Message }{}
        };
    }
}