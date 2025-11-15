//HTTPReader.hpp
#pragma once
#include <string>
#include "HTTPParser.hpp"
#include "winsock2.h"
namespace MyHTTP {

    //Reads HTTP Request from socket. Owns socket and closes it by himself
    class HTTPReader {
    public:
        HTTPReader(SOCKET iClientSocket) : _socket{ iClientSocket } {
            _HTTPRequest = ReadHTTPRequest();
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

        ~HTTPReader() {
            if (_socket != INVALID_SOCKET) {
                closesocket(_socket);
            }
        }
        HTTPRequest GetHTTPRequest() {
            return _HTTPRequest;
        }

    private:
        HTTPRequest ReadHTTPRequest();
        HTTPRequest _HTTPRequest;
        SOCKET _socket;
    };

}