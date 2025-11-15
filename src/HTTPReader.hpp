//HTTPReader.hpp
#pragma once
#include <string>
#include "winsock2.h"
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

        void operator=(const SOCKET& s) = delete;

        HTTPReader(HTTPReader&& other) : _socket(other._socket) {
            other._socket = INVALID_SOCKET;
        }

        HTTPReader& operator=(HTTPReader&& other) {
            if (this != &other) {
                if (_socket != INVALID_SOCKET) {
                    closesocket(_socket);
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
        std::optional<HTTPRequest> ReadHTTPRequest();
        std::optional<HTTPRequest> _HTTPRequest;
        SOCKET _socket;
    };

}