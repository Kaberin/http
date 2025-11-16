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
        std::optional<HTTPRequest> ReadHTTPRequest(const Socket& iSocket);
    };

}