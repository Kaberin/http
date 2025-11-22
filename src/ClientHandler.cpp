#include "ClientHandler.hpp"
#include <string>
#include <chrono>
#include <thread>
#include "./HTTP/HTTPReader.hpp"
#include "Utils.hpp"
#include "./Exceptions/Exceptions.hpp"
namespace web {
    void ClientHandler::operator()(const Router& iRouter)
    {
        std::cout << "Socket ID: " << _socket.GetRawSocket() << " in thread " << std::this_thread::get_id() << '\n';
        std::cout << "Client Handler start\n";
        while (true) {
            std::optional <HTTPRequest> requestOptional;
            try {
                requestOptional = _socket.GetHTTPRequest();
            }
            catch (std::exception& e) {
                ExceptionHandler(e);
                break;
            };

            if (requestOptional.has_value()) {
                auto response = iRouter.Match(requestOptional.value());
                _socket.Send(response.ToString());
                if (response._headers[HeaderNames::CONNECTION] == HeaderValues::CONNECTION_CLOSE) {
                    break;
                }
            }
        }
        std::cout << "End of ClientHandler process.\n";
    }
}