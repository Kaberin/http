#pragma once
#include <winsock2.h>
#include <iostream>
#include <optional>
#include "WebTypes.hpp"

namespace web
{

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

    class Socket {
    public:
        Socket(SOCKET iRawSocket, SocketType iSocketType) : _socket{ iRawSocket }, _socketType{ iSocketType } {}
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
        const SOCKET& GetRawSocket();
        ~Socket();
    private:
        SOCKET _socket;
        SocketType _socketType;
    };

}