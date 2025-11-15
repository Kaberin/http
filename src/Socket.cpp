#include "Socket.hpp"
#include "HTTPReader.hpp"
#include "Utils.hpp"
namespace web
{
    Socket::Socket(int iServerPort, int backlog) {
        _socketType = SocketType::Server;
        SOCKADDR_IN addr;
        addr.sin_addr.S_un.S_addr = INADDR_ANY;
        addr.sin_port = htons(iServerPort);
        addr.sin_family = AF_INET;
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        bind(_socket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
        listen(_socket, backlog);
    }

    Socket::~Socket() {
        if (_socket != INVALID_SOCKET) {
            closesocket(_socket);
        }
    }

    const SOCKET& Socket::GetRawSocket() {
        return _socket;
    }

    std::optional<Socket> Socket::AcceptConnection() {
        if (_socketType == SocketType::Server) {
            Socket clientSocket(accept(_socket, nullptr, nullptr), SocketType::Client);
            return clientSocket;
        }
        return std::nullopt;
    }

    std::optional<HTTPRequest> Socket::GetHTTPRequest() {
        if (_socketType == SocketType::Client) {
            HTTPReader reader(*this);
            reader.ReadNewHttpRequest();
            return reader.GetHTTPRequest();
        }
        else {
            throw std::runtime_error{ "Only client sockets can get http requests!" };
        }

    }
}