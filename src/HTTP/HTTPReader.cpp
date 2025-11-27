//HTTPReader.cpp
#include "HTTPReader.hpp"
#include <cmath>
#include "../Utils.hpp"
#include "../Exceptions/Exceptions.hpp"
namespace web
{
    void HandleChunked()
    {
    
    }

    std::optional<HTTPRequest> HTTPReader::ReadHTTPRequest(const Socket& iSocket)
    {
        std::string iRawRequest;
        std::string line = "";
        try {
            std::string stop = "\r\n\r\n";
            line = iSocket.Read(stop);
        }
        catch (exceptions::HTTPException& e) {
            throw;
        }
        if (line.empty()) return std::nullopt;
        iRawRequest += line;
        HTTPParser reader(iRawRequest);
        reader.ParseHTTPRequestUntilBody();
        auto parsedrequest = reader.GetRequest();
        HTTPRequest resultRequest;
        if (parsedrequest.has_value()) {
            auto request = parsedrequest.value();
            request.GetHeader("Transfer-Encoding").has_value();
            auto contentLengthStr = "Content-Length";
            bool isContentLength = request._headers.find(contentLengthStr) != request._headers.end();
            bool isLengthANumber = isContentLength ? Utils::IsInteger(request._headers[contentLengthStr]) : false;
            int contentLength = isLengthANumber ? Utils::StringToInteger(request._headers[contentLengthStr]) : 0;
            auto headerEnd = iRawRequest.find("\r\n\r\n") + 4;
            std::string bodyInBuffer = iRawRequest.substr(headerEnd);
            int remainingBytes = contentLength - bodyInBuffer.size();
            try {
                bodyInBuffer += iSocket.Read(remainingBytes);
            }
            catch (exceptions::HTTPException& e) {
                throw;
            }
            resultRequest = request;
            resultRequest._body = bodyInBuffer;
            return resultRequest;
        }
        return std::nullopt;
    }
}