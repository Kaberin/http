// HTTPReader.cpp
#include "HTTPReader.hpp"

#include <cmath>

#include "../DefaulValues.hpp"
#include "../Exceptions/Exceptions.hpp"
#include "../Utils.hpp"
namespace web {
    std::string HandleChunked(const Socket& iSocket) {
        std::string body;
        const std::string CLRF = "\r\n";
        while (true) {
            auto line = iSocket.Read(CLRF);
            try {
                int chunkSize = std::stoi(line, nullptr, 16);
                std::cout << "Current chunk size: " << chunkSize << '\n';
                if (chunkSize == 0) {
                    break;
                }
                std::string chunk = iSocket.Read(chunkSize);
                std::cout << "Read chunk: " << chunk << "\n"
                          << "Size of chunk is " << chunk.size() << "\n";
                std::cout << "Remained buffer: " << iSocket.GetBuffer() << '\n';
                if (!chunk.empty()) {
                    body.append(chunk);
                }
                iSocket.Read(CLRF);
            } catch (const std::exception& e) {
                break;
            }
        }

        try {
            if (!iSocket.GetBuffer().empty() || iSocket.HasData()) {
                iSocket.Read(CLRF);
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << '\n';
        }
        return body;
    }

    std::optional<HTTPRequest> HTTPReader::ReadHTTPRequest(
        const Socket& iSocket) {
        std::string iRawRequest;
        std::string line = "";
        try {
            std::string stop = "\r\n\r\n";
            line = iSocket.Read(stop);
        } catch (exceptions::HTTPException& e) {
            throw;
        }
        if (line.empty()) return std::nullopt;
        iRawRequest += line;
        HTTPParser reader(iRawRequest);
        reader.ParseHTTPRequestUntilBody();
        auto parsedrequest = reader.GetRequest();
        HTTPRequest resultRequest;
        if (parsedrequest.has_value()) {
            auto& request = parsedrequest.value();
            std::cout << request << '\n';
            // auto headerEnd = iRawRequest.find("\r\n\r\n") + 4;
            std::string bodyInBuffer;
            if (request.GetHeader(HeaderNames::TRANSFER_ENCODING).has_value() &&
                request.GetHeader(HeaderNames::TRANSFER_ENCODING).value() ==
                    HeaderValues::TRANSFER_ENCODING_CHUNK) {
                std::cout << "Buffer start:\n"
                          << iSocket.GetBuffer() << "\nBuffer end.\n";
                bodyInBuffer.append(HandleChunked(iSocket));
            } else {
                auto contentLengthStr = "Content-Length";
                bool isContentLength =
                    request._headers.find(contentLengthStr) !=
                    request._headers.end();
                bool isLengthANumber =
                    isContentLength
                        ? Utils::IsInteger(request._headers[contentLengthStr])
                        : false;
                int contentLength =
                    isLengthANumber ? Utils::StringToInteger(
                                          request._headers[contentLengthStr])
                                    : 0;
                int remainingBytes = contentLength - bodyInBuffer.size();
                try {
                    bodyInBuffer += iSocket.Read(remainingBytes);
                } catch (exceptions::HTTPException& e) {
                    throw;
                }
            }
            resultRequest = request;
            resultRequest._body = bodyInBuffer;
            std::cout << resultRequest << '\n';
            return resultRequest;
        }
        return std::nullopt;
    }
}  // namespace web