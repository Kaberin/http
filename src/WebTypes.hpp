#pragma once
#include <map>
#include <string>

#include "MagicEnum.hpp"

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

    enum class HTTPVersion {
        HTTP1_1,
        HTTP1_0
    };

    enum class ContentType {
        APPLICATION_JSON,
        TEXT_PLAIN
    };

    enum class StatusCode {
        Ok = 200,
        BadRequest = 400,
        NotFound = 404,
        NotAllowed = 405,
        Timeout = 408,
        InternalServerError = 500,
    };

    enum class FileType {
        HTML, 
        CSS,
        JS,
        INVALID
    };

    struct HTTPResponse {
        HTTPVersion _version;
        StatusCode _statusCode;
        std::map<std::string, std::string> _headers;
        std::string _body;

        std::string ToString() {
            std::string result = "";
            const std::string CLRF = "\r\n";
            std::string version = _version == HTTPVersion::HTTP1_1 ? "HTTP/1.1" : "HTTP/1.0";
            std::string message{ magic_enum::enum_name(_statusCode) };
            result = version + " " + std::to_string(static_cast<int>(_statusCode)) + " " + message + CLRF;
            std::string headers = "";
            if (!_body.empty()) {
                _headers["Content-Length"] = std::to_string(_body.size());
            }
            for (const auto& [header, headerValue] : _headers) {
                headers += (header + ": " + headerValue + CLRF);
            }
            result += headers;
            result += CLRF;
            if (!_body.empty()) {
                result += _body;
            }
            return result;
        }
    };
}