//HTTPParser.hpp
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <sstream>
#include "libs/magic_enum/magic_enum.hpp"


namespace MyHTTP
{
    class HTTPReader;

    enum class HTTPMethod {
        GET,
        POST,
        EMPTY
    };
    enum class HTTPConnection {
        Close,
        KeepAlive
    };

    struct HTTPRequest {
        HTTPMethod _method;
        std::string _path;
        std::string _version;
        std::map<std::string, std::string> _headers;
        std::string _body;
    };
    //Class that parses http request string
    //You can get parsed http by GetRequest() which returns std::optional<HTTPRequest>
    class HTTPParser {
    public:
        HTTPParser(const std::string& rawString) : _rawString{ rawString } {}
        std::optional<HTTPRequest> GetRequest() {
            return _request;
        }
        friend HTTPReader;
        void ParseHTTPRequestUntilBody();
    private:
        std::map<std::string, std::string> ParseHTTPHeaders(std::stringstream& iStream);
        std::optional<HTTPRequest> _request;
        const std::string& _rawString;
    };
}
