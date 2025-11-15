#pragma once
#include <string>

#include "HTTPParser.hpp"

namespace web
{
    namespace Utils {
        int StringToInteger(const std::string& iString);
        int IsInteger(const std::string& iString);
    }

    std::ostream& operator<<(std::ostream& os, const HTTPRequest& req);
    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& map);
}