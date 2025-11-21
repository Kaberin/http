#pragma once
#include "src/Utils.hpp"
#include "src/HTTP/Socket.hpp"
#include "src/HTTP/HTTPReader.hpp"
#include "src/ClientHandler.hpp"
#include "src/WebTypes.hpp"
#include "src/Router/Router.hpp"


namespace web {
    class Server {
    public:
        Server(int iPort, Router iRouter);
        void Start();
    private:
        Router _router;
        int _port;
    };
}
