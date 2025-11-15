#include "ClientHandler.hpp"
#include <string>
#include <chrono>

#include "HTTPReader.hpp"
#include "Utils.hpp"
namespace web {
    void ClientHandler::operator()()
    {
        std::cout << "Client Handler start\n";
        auto start = std::chrono::steady_clock::now();
        auto lastPrint = start;
        auto timeout = std::chrono::seconds(10);
        int counter = 0;
        while (true) {
            auto requestOpt = _socket.GetHTTPRequest();
            if (requestOpt.has_value()) {
                auto request = requestOpt.value();
                _socket.SendHTTPResponse();
                std::cout << request << '\n';
                if (request._headers["Connection"] == "close") {
                    break;
                }
            }
        }
    }
}