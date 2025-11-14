#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>

int main(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        return 1;
    }

    SOCKADDR_IN addr; 
    addr.sin_addr.S_un.S_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    addr.sin_family = AF_INET;

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(addr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Listening on port 8080...\n";
    int size = sizeof(addr);
    SOCKET client = accept(serverSocket, nullptr, nullptr);;

    char buf[1024];
    recv(client, buf, sizeof(buf), 0);
    std::cout << "Client message:\n" << buf << '\n'; 
    std::stringstream ss{buf};
    std::string word;
    std::string page;
    while(ss >> word){
        if(word != "GET"){
            continue;
        }
        ss >> page;
        break;
    }

    std::string msg =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 20\r\n"
        "\r\n"
        "Hello, World!";
    
    msg += page;
    send(client, msg.c_str(), strlen(msg.c_str()), 0);

    closesocket(client);
    WSACleanup();
    return 0;
}