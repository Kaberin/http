//HTTPRequest.cpp
#include "./HTTPParser.hpp"

#include <sstream>
#include "../Utils.hpp"

#include "../../libs/nlohmann/json.hpp"
namespace web
{
    void HTTPParser::ParseHTTPRequestUntilBody()
    {
        if (_rawString.empty()) {
            return;
        }
        HTTPRequest request;
        std::stringstream HTTPStream{ _rawString };
        std::string startLine;
        if (!std::getline(HTTPStream, startLine)) {
            return;
        }
        if (!startLine.empty() && startLine.back() == '\r') {
            startLine.pop_back();
        }
        std::istringstream lineStream(startLine);
        std::string method, path, version;
        lineStream >> method >> path >> version;
        request._method = magic_enum::enum_cast<HTTPMethod>(method).value_or(HTTPMethod::EMPTY);
        request._path = path;
        request._version = version;
        request._headers = ParseHTTPHeaders(HTTPStream);
        _request = request;
    }

    std::map<std::string, std::string> HTTPParser::ParseHTTPHeaders(std::stringstream& iStream)
    {
        std::map<std::string, std::string> headers;
        std::string line;

        while (std::getline(iStream, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) {
                break;
            }

            auto colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                continue;
            }

            std::string headerName = line.substr(0, colonPos);
            std::string headerContent = line.substr(colonPos + 1);
            headerContent.erase(0, headerContent.find_first_not_of(" \t"));

            headers[headerName] = headerContent;
        }

        return headers;
    }
}