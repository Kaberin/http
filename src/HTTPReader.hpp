//HTTPReader.hpp
#pragma once
#include <string>
#ifdef _WIN32

#include "winsock2.h"
using socket_t = SOCKET;

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using socket_t = int;

#endif

#include "Socket.hpp"
#include "HTTPParser.hpp"
namespace web
{
    //Reads HTTP Request from socket. Owns socket and closes it by himself
    class HTTPReader {
    public:
        HTTPReader(Socket& iClientSocket) : _socket{ iClientSocket.GetRawSocket() } {
            //_HTTPRequest = ReadHTTPRequest();
        };

        void operator=(const socket_t& s) = delete;

        HTTPReader(HTTPReader&& other) : _socket(other._socket) {
            other._socket = INVALID_SOCKET;
        }

        HTTPReader& operator=(HTTPReader&& other) {
            if (this != &other) {
                if (_socket != INVALID_SOCKET) {
#ifdef _WIN32
                    closesocket(_socket);
#else
                    close(_socket);
#endif
                    _socket = other._socket;
                    other._socket = INVALID_SOCKET;
                }
            }
            return *this;
        }

        friend Socket;
    private:
        void ReadNewHttpRequest();

        std::optional<HTTPRequest> GetHTTPRequest() {
            return _HTTPRequest;
        }

    private:
        HTTPRequest ReadHTTPRequest();
        HTTPRequest _HTTPRequest;
        socket_t _socket;
    };

}