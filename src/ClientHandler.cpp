#include "ClientHandler.hpp"

#include <chrono>
#include <string>
#include <thread>

#include "./Exceptions/Exceptions.hpp"
#include "./HTTP/HTTPReader.hpp"
#include "Utils.hpp"
namespace web {
    void ClientHandler::operator()(const Router& iRouter) {
        Logger::GetInstance().Log(LogType::INFO,
                                  "Client handler started work!");
        while (true) {
            std::optional<HTTPRequest> requestOptional;
            try {
                requestOptional = _socket.GetHTTPRequest();
            } catch (std::exception& e) {
                ExceptionHandler(e);
                break;
            };

            if (requestOptional.has_value()) {
                auto response = iRouter.Match(requestOptional.value());
                _socket.Send(response.ToString());
                if (response._headers[HeaderNames::CONNECTION] ==
                    HeaderValues::CONNECTION_CLOSE) {
                    break;
                }
            }
        }
        Logger::GetInstance().Log(LogType::INFO,
                                  "End of ClientHandler process");
    }
}  // namespace web