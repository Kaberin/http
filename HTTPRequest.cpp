//HTTPRequest.cpp
#include "HTTPRequest.hpp"

#include <sstream>
#include "libs/nlohmann/json.hpp"

namespace MyHTTP
{
    std::optional<HTTP::HTTPRequest> HTTP::ParseHTTPRequest(const std::string& iRawHttp)
    {
        if (iRawHttp.empty()) {
            return std::nullopt;
        }
        HTTPRequest request;
        std::stringstream HTTPStream{ iRawHttp };
        std::string startLine;
        if (!std::getline(HTTPStream, startLine)) {
            return std::nullopt;
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
        return request;
    }

    std::map<std::string, std::string> HTTP::ParseHTTPHeaders(std::stringstream& iStream) {
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

    std::ostream& operator<<(std::ostream& os, const HTTP::HTTPRequest& req)
    {
        std::string method{ magic_enum::enum_name(req._method) };
        os << "Method: " << method << '\n'
            << "Path: " << req._path << '\n'
            << "Version: " << req._version << '\n'
            << "Headers" << req._headers << '\n';
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& map) {
        os << " {\n";
        for (const auto& [key, value] : map) {
            os << '\t' << key << ": " << value << '\n';
        }
        os << "}";
        return os;
    }
}