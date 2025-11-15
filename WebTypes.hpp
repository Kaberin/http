#pragma once
#include <map>
#include <string>

namespace web
{
    enum class HTTPMethod {
        GET,
        POST,
        PUT,
        DEL,
        EMPTY
    };

    enum class HTTPConnection {
        Close,
        KeepAlive
    };


    enum class SocketType {
        Client,
        Server,
    };

    struct HTTPRequest {
        HTTPMethod _method;
        std::string _path;
        std::string _version;
        std::map<std::string, std::string> _headers;
        std::string _body;
    };
}