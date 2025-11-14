//Server.cpp
#include <iostream>
#include "HTTPRequest.hpp"


int main() {
    // std::string request{
    //                     "GET /products?id=123&sort=asc HTTP/1.1\r\n"
    //                     "Host: example.com\r\n"
    //                     "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64)\r\n"
    //                     "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    //                     "Accept-Language: en-US,en;q=0.5\r\n"
    //                     "Accept-Encoding: gzip, deflate\r\n"
    //                     "Connection: keep-alive\r\n"
    //                     "Upgrade-Insecure-Requests: 1\r\n"
    // };
    std::string rawHttp =
        "GET /products?id=123 HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "User-Agent: TestClient/1.0\r\n"
        "Accept: text/html\r\n"
        "Connection: close\r\n"
        "\r\n";
    //std::cout << rawHttp << "\n\n\n";
    MyHTTP::HTTP http(rawHttp);
    std::cout << http.GetRequest() << '\n';

    return 0;
}