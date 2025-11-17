#include "Socket.hpp"
#include "HTTPReader.hpp"
#include "../Utils.hpp"
#include "../DefaulValues.hpp"
#include "../Exceptions/Exceptions.hpp"
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
            if (readBytes == 0) {
                break;
            }
            if (readBytes < 0) {
#ifdef _WIN32
                auto err = WSAGetLastError();
                if (err == WSAETIMEDOUT) {
                    throw exceptions::Timeout();
                }
                throw exceptions::SocketError();
#else
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    throw exceptions::Timeout();
                }
                throw exceptions::SocketError();
#endif
            }
            result.append(buf, readBytes);
            bytesToRead -= readBytes;
        }
        return result;
    }

    std::string Socket::Read(std::string& iStopMark) const
    {
        char buf[1024];
        int readBytes = 0;
        std::string result;
        while (true) {
            readBytes = recv(_socket, buf, sizeof(buf), 0);

            if (readBytes == 0) {
                break;
            }

            if (readBytes < 0) {
#ifdef _WIN32
                auto err = WSAGetLastError();
                if (err == WSAETIMEDOUT) {
                    throw exceptions::Timeout();
                }
                throw exceptions::SocketError();
#else
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    throw exceptions::Timeout();
                }
                throw exceptions::SocketError();
#endif
            }

            result.append(buf, readBytes);

            if (result.find(iStopMark) != std::string::npos) {
                break;
            }
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
#ifdef _WIN32
            DWORD timeout = DEFAULT_VALUES::SOCKET_TIMEOUT * 1000;
            setsockopt(rawSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#else
            struct timeval tv;
            tv.tv_sec = DEFAULT_VALUES::SOCKET_TIMEOUT;
            tv.tv_usec = 0;
            setsockopt(rawSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif
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