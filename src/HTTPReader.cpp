//HTTPReader.cpp
#include "HTTPReader.hpp"
#include "Utils.hpp"
#include <cmath>
namespace web
{
    HTTPRequest HTTPReader::ReadHTTPRequest()
    {
        std::string iRawRequest;

        char buf[1024];
        int bytes = 0;
        while ((bytes = recv(_socket, buf, sizeof(buf), 0)) > 0) {
            iRawRequest.append(buf, bytes);
            if (iRawRequest.find("\r\n\r\n") != std::string::npos) {
                break;
            }
        }
        HTTPParser reader(iRawRequest);
        reader.ParseHTTPRequestUntilBody();
        auto parsedrequest = reader.GetRequest();
        HTTPRequest resultRequest;
        if (parsedrequest.has_value()) {
            auto request = parsedrequest.value();
            bool isContentType = request._headers.find("Content-Type") != request._headers.end();
            auto contentLengthStr = "Content-Length";
            bool isContentLength = request._headers.find(contentLengthStr) != request._headers.end();
            bool isLengthANumber = isContentLength ? Utils::IsInteger(request._headers[contentLengthStr]) : false;
            int contentLength = isLengthANumber ? Utils::StringToInteger(request._headers[contentLengthStr]) : 0;

            auto headerEnd = iRawRequest.find("\r\n\r\n") + 4;
            std::string bodyInBuffer = iRawRequest.substr(headerEnd);
            int remainingBytes = contentLength - bodyInBuffer.size();

            while (remainingBytes > 0) {
                bytes = recv(_socket, buf, (std::min)(remainingBytes, static_cast<int>(sizeof(buf))), 0);
                if (bytes <= 0) break;
                bodyInBuffer.append(buf, bytes);
                remainingBytes -= bytes;
            }
            resultRequest = request;
            resultRequest._body = bodyInBuffer;
        }
        return resultRequest;
    }
}