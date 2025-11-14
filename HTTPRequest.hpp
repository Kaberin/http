//HTTPRequest.hpp
#pragma once
#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <sstream>
#include "libs/magic_enum/magic_enum.hpp"


namespace MyHTTP
{
    class HTTP {
    public:
        HTTP(const std::string& rawString) {
            auto parsedRequest = ParseHTTPRequest(rawString);
            if (!parsedRequest.has_value()) {
                throw std::runtime_error{ "Invalid RawString" };
            }
            _request = parsedRequest.value();
        }

        enum class HTTPMethod {
            GET,
            POST,
            EMPTY
        };
        struct HTTPRequest {
            HTTPMethod _method;
            std::string _path;
            std::string _version;
            std::map<std::string, std::string> _headers;
            std::string _body;
        };
        enum class HTTPConnection {
            Close,
            KeepAlive
        };

        HTTPRequest GetRequest() {
            return _request;
        }
    private:
        std::optional<HTTPRequest> ParseHTTPRequest(const std::string& iRawHttp);
        std::map<std::string, std::string> ParseHTTPHeaders(std::stringstream& iStream);
        HTTPRequest _request;
    };

    std::ostream& operator<<(std::ostream& os, const HTTP::HTTPRequest& req);
    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& map);




}
