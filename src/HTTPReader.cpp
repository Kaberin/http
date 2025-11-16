//HTTPReader.cpp
#include "HTTPReader.hpp"
#include "Utils.hpp"
#include <cmath>
namespace web
{
    std::optional<HTTPRequest> HTTPReader::ReadHTTPRequest(const Socket& iSocket)
    {
        std::string iRawRequest;
        int bytesChunk = 1024;
        std::string stop = "\r\n\r\n";
        std::string line = iSocket.Read(bytesChunk, stop);
        if (line.empty()) return std::nullopt;
        iRawRequest += line;
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
            bodyInBuffer += iSocket.Read(remainingBytes);
            resultRequest = request;
            resultRequest._body = bodyInBuffer;
            return resultRequest;
        }
        return std::nullopt;
    }
}