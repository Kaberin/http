#include "ClientHandler.hpp"
#include <string>
#include <chrono>
#include <thread>
#include "HTTPReader.hpp"
#include "Utils.hpp"
#include "Exceptions.hpp"
namespace web {
    void ClientHandler::operator()()
    {
        std::cout << "Socket ID: " << _socket.GetRawSocket() << " in thread " << std::this_thread::get_id() << '\n';
        std::cout << "Client Handler start\n";
        auto start = std::chrono::steady_clock::now();
        auto lastPrint = start;
        auto timeout = std::chrono::seconds(10);
        int counter = 0;
        while (true) {
            std::optional <HTTPRequest> requestOpt;
            try {
                requestOpt = _socket.GetHTTPRequest();
            }
            catch (std::exception& e) {
                std::cout << e.what() << "\n" << "Closing connection\n" << '\n';
                HTTPResponse response{
                       HTTPVersion::HTTP1_1,
                       StatusCode::Timeout,
                       {
                           {"Connection", "close"},
                           {"Content-Type", "text/plain"},
                       },
                       e.what()
                };
                _socket.Send(response.ToString());
                break;
            };
            if (requestOpt.has_value()) {
                auto request = requestOpt.value();
                std::cout << "Current HTTP Request: \n" << request << "\n\n";
                if (!request._body.empty()) {
                    HTTPResponse response {
                        HTTPVersion::HTTP1_1,
                        StatusCode::Ok,
                        {
                            {"Connection", "keep-alive"},
                            {"Content-Type", "text/plain"},
                            {"chupapimunanya", "baza" }
                        },
                        request._body
                    };
                     _socket.Send(response.ToString());
                }
                else {
                   HTTPResponse response{
                   HTTPVersion::HTTP1_1,
                   StatusCode::Ok,
                   {
                       {"Connection", "keep-alive"},
                       {"Content-Type", "text/plain"},
                       {"chupapimunanya", "baza" }
                   },
                   "No body..."
                    };
                    _socket.Send(response.ToString());
                }
                if (request._headers["connection"] == "close" || request._headers["Connection"] == "close") {
                    std::cout << "Conection is closed by client.\n";
                    break;
                }
            }
            else {
            }
        }
        std::cout << "End of ClientHandler process.\n";
    }
}