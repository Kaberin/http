//Server.cpp
#include <iostream>
#include "HTTPReader.hpp"
#include "Utils.hpp"

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 1;
    }

    SOCKADDR_IN addr;
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
    listen(serverSocket, SOMAXCONN);
    auto clientSocket = accept(serverSocket, nullptr, nullptr);
    MyHTTP::HTTPReader reader(clientSocket);
    auto req = reader.GetHTTPRequest();

    // MyHTTP::HTTPParser httpParser(rawHttp);
    // auto parsedRequest = httpParser.GetRequest().has_value() ? httpParser.GetRequest().value() : MyHTTP::HTTPParser::HTTPRequest{};
    std::cout << req << '\n';

    return 0;
}