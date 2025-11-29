#include "Socket.hpp"

#include "HTTPReader.hpp"
#include "../Utils.hpp"
#include "../DefaulValues.hpp"
#include "../Exceptions/Exceptions.hpp"
namespace web
{
    std::chrono::seconds Socket::_idleTtimer = std::chrono::seconds(DefaultValues::IDLE_TIMEOUT);

    Socket::Socket(int iServerPort, SocketType iSocketType, int backlog) {
        //std::cout << "Server socket constructor...\n";
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

    Socket::Socket(socket_t iRawSocket, SocketType iSocketType) : _socket{ iRawSocket }, _socketType{ iSocketType } {
        //std::cout << "Client socket constructor...\n";
    }

    Socket::Socket(Socket&& iSocket) noexcept : _socket{ iSocket._socket }, _socketType{ iSocket._socketType } {
        iSocket._socket = INVALID_SOCKET;
    }

    Socket& Socket::operator=(Socket&& iSocket) noexcept {
        //std::cout << "Move assignment...\n";
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

    Socket::~Socket() {
        if (_socket != INVALID_SOCKET) {
            std::cout << "Closing socket " << _socket << '\n';
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

    void Socket::PutBack(const std::string& iPutback) const
    {
        _buffer = iPutback + _buffer;
    }

    std::string Socket::Read(size_t bytes) const {
        int bytesToRead = bytes;
        char buf[1024];

        int readBytes = 0;
        std::string result;

        if (!_buffer.empty()) {
            size_t take = (std::min)(bytes, _buffer.size());
            result += _buffer.substr(0, take);
            _buffer.erase(0, take);
            bytesToRead -= take;
        }

        while (bytesToRead > 0) {
            readBytes = recv(_socket, buf, (std::min)(bytesToRead, static_cast<int>(sizeof(buf))), 0);
            if (readBytes == 0) {
                break;
            }
            if (readBytes < 0) {
                ProcessException();
            }
            result.append(buf, readBytes);
            bytesToRead -= readBytes;
        }
        return result;
    }

    std::string Socket::Read(const std::string& iStopMark) const
    {
        char buf[1024];
        int readBytes = 0;
        std::string result;
        auto last_activity = Clock::now();
        while (true) {
            auto stopMarkPos = _buffer.find(iStopMark);
            if (stopMarkPos != std::string::npos) {
                result += _buffer.substr(0, stopMarkPos);
                _buffer.erase(0, stopMarkPos + iStopMark.size());
                return result;
            }
            if (!HasData()) {
                auto now = Clock::now();
                if (now - last_activity >= _idleTtimer) {
                    throw exceptions::IdleTimeout();
                }
                continue;
            }
            readBytes = recv(_socket, buf, sizeof(buf), 0);

            if (readBytes == 0) {
                break;
            }
            if (readBytes < 0) {
                ProcessException();
            }

            _buffer.append(buf, readBytes);
            last_activity = Clock::now();
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
            //std::cout << "Accepted socket: " << rawSocket << '\n';
#ifdef _WIN32
            DWORD timeout = DefaultValues::SOCKET_TIMEOUT * 1000;
            setsockopt(rawSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#else
            struct timeval tv;
            tv.tv_sec = DefaultValues::SOCKET_TIMEOUT;
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


    bool Socket::HasData() const
    {
#ifdef _WIN32
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(_socket, &readfds);
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        int ret = select(0, &readfds, nullptr, nullptr, &tv);
        if (ret == SOCKET_ERROR)
            return false;
        int data = FD_ISSET(_socket, &readfds);
        if (data == 0) 
            return false;
        return true;
#else
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(_socket, &readfds);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int ret = select(_socket + 1, &readfds, nullptr, nullptr, &tv);

        if (ret < 0)
            return false;

        return FD_ISSET(_socket, &readfds);
#endif
    }

    void Socket::ProcessException() const
    {
#ifdef _WIN32
        auto err = WSAGetLastError();
        if (err == WSAETIMEDOUT) {
            throw exceptions::SocketTimeout();
        }
        throw exceptions::SocketError();
#else
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            throw exceptions::SocketTimeout();
        }
        throw exceptions::SocketError();
#endif
    }
}