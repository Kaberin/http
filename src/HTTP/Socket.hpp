#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <optional>

#include "../WebTypes.hpp"

using socket_t = int;
#define INVALID_SOCKET -1
namespace web {

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
        std::string Read(size_t bytes) const;
        std::string Read(const std::string& iStopMark) const;
        bool HasData() const;
        socket_t GetRawSocket() const;
        ~Socket();
        void PutBack(const std::string&) const;

        const std::string& GetBuffer() const { return _buffer; }

       private:
        void ProcessException() const;

        socket_t _socket;
        SocketType _socketType;
        static std::chrono::seconds _idleTtimer;

        mutable std::string _buffer;
    };
}  // namespace web