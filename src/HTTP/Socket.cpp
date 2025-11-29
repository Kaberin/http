#include "Socket.hpp"

#include "../DefaulValues.hpp"
#include "../Exceptions/Exceptions.hpp"
#include "../Utils.hpp"
#include "HTTPReader.hpp"
#include "Logger/Logger.hpp"
namespace web {
    std::chrono::seconds Socket::_idleTtimer =
        std::chrono::seconds(DefaultValues::IDLE_TIMEOUT);

    Socket::Socket(int iServerPort, SocketType iSocketType, int backlog) {
        // std::cout << "Server socket constructor...\n";
        _socketType = SocketType::Server;
        sockaddr_in addr;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(iServerPort);
        addr.sin_family = AF_INET;
        _socket = socket(AF_INET, SOCK_STREAM, 0);
        bind(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        listen(_socket, backlog);
    }

    Socket::Socket(socket_t iRawSocket, SocketType iSocketType)
        : _socket{iRawSocket}, _socketType{iSocketType} {
        // std::cout << "Client socket constructor...\n";
    }

    Socket::Socket(Socket&& iSocket) noexcept
        : _socket{iSocket._socket}, _socketType{iSocket._socketType} {
        iSocket._socket = INVALID_SOCKET;
    }

    Socket& Socket::operator=(Socket&& iSocket) noexcept {
        if (_socket != INVALID_SOCKET) {
            close(_socket);
        }
        _socket = iSocket._socket;
        _socketType = iSocket._socketType;
        iSocket._socket = INVALID_SOCKET;
        return *this;
    };

    Socket::~Socket() {
        if (_socket != INVALID_SOCKET) {
            std::cout << "Closing socket " << _socket << '\n';
            close(_socket);
        }
    }

    socket_t Socket::GetRawSocket() const { return _socket; }

    void Socket::PutBack(const std::string& iPutback) const {
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
            readBytes =
                recv(_socket, buf,
                     (std::min)(bytesToRead, static_cast<int>(sizeof(buf))), 0);
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

    std::string Socket::Read(const std::string& iStopMark) const {
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
            struct timeval tv;
            tv.tv_sec = DefaultValues::SOCKET_TIMEOUT;
            tv.tv_usec = 0;
            setsockopt(rawSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
            std::string logMessage =
                "Accepted connection: Socket: " + std::to_string(rawSocket) +
                '\n';
            Logger::GetInstance().Log(LogType::INFO, logMessage);
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
        } else {
            throw std::runtime_error{
                "Only client sockets can get http requests!"};
        }
    }

    bool Socket::HasData() const {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(_socket, &readfds);

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int ret = select(_socket + 1, &readfds, nullptr, nullptr, &tv);

        if (ret < 0) return false;

        return FD_ISSET(_socket, &readfds);
    }

    void Socket::ProcessException() const {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            throw exceptions::SocketTimeout();
        }
        throw exceptions::SocketError();
    }
}  // namespace web