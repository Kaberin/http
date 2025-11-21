#pragma once
#include <string>
#include "WebTypes.hpp"
namespace web {
    namespace DEFAULT_VALUES {
        static int SOCKET_TIMEOUT = 5; //seconds
        static int IDLE_TIMEOUT = 15; //seconds
        static int MAX_HEADER_SIZE = 8 * 1000; //bytes

        static HTTPResponse NOT_ALLOWED_RESPONSE = {
            HTTPVersion::HTTP1_1, 
            StatusCode::NotAllowed,
            {
                {"Connection", "close"}
            }, 
            "Not allowed"
        };
    }
}