#include "ClientHandler.hpp"
#include <string>
#include <chrono>
namespace web {
    void ClientHandler::operator()()
    {
        std::cout << "Client Handler start\n";
        auto start = std::chrono::steady_clock::now();
        auto lastPrint = start;
        auto timeout = std::chrono::seconds(10);
        int counter = 0;
        if (_request._headers["Connection"] == "keep-alive") {
            while (true) {
                auto currentTime = std::chrono::steady_clock::now();
                if (currentTime - start >= timeout) {
                    std::cout << "Connection closed\n";
                    break;
                }
                if (currentTime - start >= std::chrono::seconds(5)) {
                    _socket.SendHTTPResponse();
                    break;
                }
                if (currentTime - lastPrint >= std::chrono::seconds(1)) {
                    std::cout << std::to_string(++counter) + " seconds passed\n";
                    lastPrint = currentTime;
                }
            }
        }
    }
}