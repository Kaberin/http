#pragma once
#ifdef _WIN32

#include "winsock2.h"
#include <ws2tcpip.h>

#include "../DefaulValues.hpp"
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
#include <chrono>
#include "../WebTypes.hpp"

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
        using Clock = std::chrono::steady_clock;
    public:
        Socket(socket_t iRawSocket, SocketType iSocketType);
        Socket(int iServerPort, SocketType iSocketType, int backlog);
        Socket(Socket& iSocket) = delete;
        Socket& operator=(Socket& iSocket) = delete;
        Socket(Socket&& iSocket) noexcept;
        Socket& operator=(Socket&& iSocket) noexcept;

        std::optional<Socket> AcceptConnection();
        std::optional<HTTPRequest> GetHTTPRequest();
        bool Send(std::string iString) const;
        std::string Read(int bytes) const;
        std::string Read(std::string& iStopMark) const;
        bool HasData() const;
        socket_t GetRawSocket() const;
        ~Socket();
    private:
        void ProcessException() const;

        socket_t _socket;
        SocketType _socketType;
        static std::chrono::seconds _idleTtimer;
    };
}