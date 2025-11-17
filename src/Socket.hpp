#pragma once
#ifdef _WIN32

#include "winsock2.h"
using socket_t = SOCKET;

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using socket_t = int;
#define INVALID_SOCKET -1

#endif
#include <iostream>
#include <optional>
#include "WebTypes.hpp"

namespace web
{
#ifdef _WIN32
    class WSAInit {
    public:
        WSAInit() {
            std::cout << "Initializing winsock library...\n";
            if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0) {
                throw std::runtime_error{ "Error initializing winsock library!\n" };
            }
            std::cout << "Winsock library successfully initialized!\n";
        }
        ~WSAInit() {
            std::cout << "Cleaning winsock library...\n";
            WSACleanup();
        }

    private:
        WSADATA _wsaData;
    };
#endif
    class Socket {
    public:
        Socket(socket_t iRawSocket, SocketType iSocketType) : _socket{ iRawSocket }, _socketType{ iSocketType } {
            std::cout << "Client socket constructor...\n";
        }
        Socket(int iServerPort, SocketType iSocketType, int backlog);
        std::optional<Socket> AcceptConnection();
        Socket(Socket& iSocket) = delete;
        Socket& operator=(Socket& iSocket) = delete;
        Socket(Socket&& iSocket) : _socket{ iSocket._socket }, _socketType{ iSocket._socketType } {
            //std::cout << "Move socket constructor...\n";e
            iSocket._socket = INVALID_SOCKET;
        }
        Socket& operator=(Socket&& iSocket) {
            std::cout << "Move assignment...\n";
#ifdef _WIN32
            if (_socket != INVALID_SOCKET)
            {
                closesocket(_socket);
            }
#else
            if (_socket != INVALID_SOCKET)
            {
                close(_socket);
            }
#endif
            _socket = iSocket._socket;
            _socketType = iSocket._socketType;
            iSocket._socket = INVALID_SOCKET;
            return *this;
        };

        std::optional<HTTPRequest> GetHTTPRequest();
        bool Send(std::string iString) const;
        std::string Read(int bytes) const;
        std::string Read(std::string& iStopMark) const;
        socket_t GetRawSocket() const;
        ~Socket();
    private:
        socket_t _socket;
        SocketType _socketType;
    };

}