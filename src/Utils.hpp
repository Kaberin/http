#pragma once
#include <string>

#include "./HTTP/HTTPReader.hpp"    

namespace web
{
    namespace Utils {
        int StringToInteger(const std::string& iString);
        int IsInteger(const std::string& iString);

        std::string ContentTypeToStr(ContentType iType);
        ContentType StrToContentType(std::string iType);
        std::string ReadFile(std::ifstream& file);
    }

    //std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& aStrings);
    std::ostream& operator<<(std::ostream& os, const HTTPRequest& req);
    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& map);
    std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& aStrings);
    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& aStrings);
}