#pragma once
#include <string>
#include "WebTypes.hpp"
namespace web {

    namespace HeaderNames {
        using str = std::string;
        static const str CONNECTION = "Connection";
        static const str CONTENT_TYPE = "Content-Type";
        static const str CONTENT_LENGTH = "Content-Length";
    }

    namespace HeaderValues {
        using str = std::string;
        static const str CONNECTION_CLOSE = "close";
        static const str CONNECTION_KEEP_ALIVE = "keep-alive";
        static const str CONTENT_TYPE_APP_JSON = "application/json";
        static const str CONTENT_TYPE_TEXT_PLAIN = "text/plain";
        static const str CONTENT_TYPE_TEXT_HTML = "text/html";
        static const str CONTENT_TYPE_TEXT_CSS = "text/css";
        static const str CONTENT_TYPE_TEXT_JS = "text/javascript";
    }

    namespace DefaultValues {
        static int SOCKET_TIMEOUT = 5; //seconds
        static int IDLE_TIMEOUT = 15; //seconds
        static int MAX_HEADER_SIZE = 8 * 1000; //bytes

        static const HTTPResponse NOT_ALLOWED_RESPONSE = {
            HTTPVersion::HTTP1_1,
            StatusCode::NotAllowed,
            {
                {HeaderNames::CONNECTION, HeaderValues::CONNECTION_CLOSE}
            },
            "Not allowed"
        };

        static const HTTPResponse BAD_REQUEST_RESPONSE = {
           HTTPVersion::HTTP1_1,
           StatusCode::BadRequest,
           {
               {HeaderNames::CONNECTION, HeaderValues::CONNECTION_CLOSE}
           },
           "Bad request"
        };

        static const HTTPResponse SERVER_ERROR_RESPONSE = {
          HTTPVersion::HTTP1_1,
          StatusCode::InternalServerError,
          {
              {HeaderNames::CONNECTION, HeaderValues::CONNECTION_CLOSE}
          },
          "Internal server error"
        };

        static const HTTPResponse DEFAULT_RESPONSE = {
            HTTPVersion::HTTP1_1,
            StatusCode::Ok,
            {
                { HeaderNames::CONNECTION, HeaderValues::CONNECTION_CLOSE },
                { HeaderNames::CONTENT_TYPE, HeaderValues::CONTENT_TYPE_TEXT_PLAIN }
            }
        };
    }


}