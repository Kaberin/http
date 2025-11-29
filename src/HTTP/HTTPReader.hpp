// HTTPReader.hpp
#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
using socket_t = int;

#include "./HTTPParser.hpp"
#include "./Socket.hpp"
namespace web {
    class HTTPReader {
       public:
        std::optional<HTTPRequest> ReadHTTPRequest(const Socket& iSocket);
    };

}  // namespace web