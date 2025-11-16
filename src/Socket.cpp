#include "Socket.hpp"
#include "HTTPReader.hpp"
#include "Utils.hpp"
namespace web
{
    Socket::Socket(int iServerPort, SocketType iSocketType, int backlog) {
        std::cout << "Server socket constructor...\n";
        _socketType = SocketType::Server;
        sockaddr_in addr;
#ifdef _WIN32
        addr.sin_addr.S_un.S_addr = INADDR_ANY;
#else
        addr.sin_addr.s_addr = INADDR_ANY;
#endif
        addr.sin_port = htons(iServerPort);
        addr.sin_family = AF_INET;
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        bind(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        listen(_socket, backlog);
    }

    Socket::~Socket() {
        if (_socket != INVALID_SOCKET) {
            std::cout << "Socket::~Socket(): Closing socket...\n";
#ifdef _WIN32
            closesocket(_socket);
#else
            close(_socket);
#endif
        }
    }

    socket_t Socket::GetRawSocket() const
    {
        return _socket;
    }

    std::string Socket::Read(int bytes) const {
        int bytesToRead = bytes;
        char buf[1024];

        int readBytes = 0;
        std::string result;
        while (bytesToRead > 0) {
            readBytes = recv(_socket, buf, (std::min)(bytesToRead, static_cast<int>(sizeof(buf))), 0);
            if (readBytes <= 0) break;
            result.append(buf, readBytes);
            bytesToRead -= readBytes;
        }
        return result;
    }

    std::string Socket::Read(int chunkSize, std::string& iStopMark) const
    {
        int bytesToRead = chunkSize;
        char buf[1024];

        int readBytes = 0;
        std::string result;
        while (bytesToRead > 0) {
            readBytes = recv(_socket, buf, (std::min)(bytesToRead, static_cast<int>(sizeof(buf))), 0);
            if (readBytes <= 0) break; 
            result.append(buf, readBytes);
            if (result.find(iStopMark) != std::string::npos) {
                bytesToRead = 0;
                continue;
            }
            bytesToRead -= readBytes;
        }
        return result;
    }

    bool Socket::Send(std::string iString) const {
        int bytesToSend = iString.size();
        int sent = 0;
        auto data = iString.c_str();
        while (bytesToSend > 0) {
            int res = send(_socket, data + sent, bytesToSend - sent, 0);
            if (res <= 0) {
                return false;
            }
            sent += res;
        }
        return true;
    }

    std::optional<Socket> Socket::AcceptConnection() {
        if (_socketType == SocketType::Server) {
            socket_t rawSocket = accept(_socket, nullptr, nullptr);
            std::cout << "Accepted socket: " << rawSocket << '\n';
            if (rawSocket == INVALID_SOCKET) {
                return std::nullopt;
            }
            return std::optional<Socket>(Socket(rawSocket, SocketType::Client));
        }
        return std::nullopt;
    }

    std::optional<HTTPRequest> Socket::GetHTTPRequest() {
        if (_socketType == SocketType::Client) {
            HTTPReader reader;
            return reader.ReadHTTPRequest(*this);
        }
        else {
            throw std::runtime_error{ "Only client sockets can get http requests!" };
        }
    }
}