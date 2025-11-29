#pragma once
#include "src/ClientHandler.hpp"
#include "src/HTTP/HTTPReader.hpp"
#include "src/HTTP/Socket.hpp"
#include "src/Router/Router.hpp"
#include "src/Utils.hpp"
#include "src/WebTypes.hpp"

namespace web {
    class Server {
       public:
        Server(int iPort, Router iRouter);
        void Start();

       private:
        Router _router;
        int _port;
    };
}  // namespace web
