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
        Socket(socket_t iRawSocket, SocketType iSocketType) : _socket{ iRawSocket }, _socketType{ iSocketType } {}
        Socket(int iServerPort, int backlog = SOMAXCONN);
        std::optional<Socket> AcceptConnection();
        Socket(Socket& iSocket) = delete;
        Socket(Socket&& iSocket) {
            _socket = iSocket._socket;
            _socketType = iSocket._socketType;
            iSocket._socket = INVALID_SOCKET;
        }
        void operator=(Socket&& iSocket) = delete;

        HTTPRequest GetHTTPRequest();
        void SendHTTPResponse() {
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 12\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Hello world!";
            send(_socket, response.c_str(), response.size(), 0);
        }
        const socket_t& GetRawSocket();
        ~Socket();
    private:
        socket_t _socket;
        SocketType _socketType;
    };

}