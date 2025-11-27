#include "Utils.hpp"
#include "../libs/nlohmann/json.hpp"
#include "../libs/magic_enum/magic_enum.hpp"
#include <algorithm>
#include <fstream>
namespace web
{
    namespace Utils
    {
        int StringToInteger(const std::string& iString) {
            return std::stoi(iString);
        }

        int IsInteger(const std::string& iString) {
            return std::all_of(iString.begin(), iString.end(), [](char c) {
                if (isdigit(c)) {
                    return true;
                }
                return false;
                });
        }

        std::string ContentTypeToStr(ContentType iType)
        {
            std::string s = std::string(magic_enum::enum_name(iType));
            std::ranges::transform(s, s.begin(), [](char c) {
                return std::tolower(c);
                });
            std::ranges::replace(s, '_', '/');
            return s;
        }

        ContentType StrToContentType(std::string iType)
        {
            std::ranges::transform(iType, iType.begin(), [](char c) {
                return std::toupper(c);
            });
            std::ranges::replace(iType, '/', '_');
            return magic_enum::enum_cast<ContentType>(iType).value_or(ContentType::TEXT_PLAIN);
        }

        std::string ReadFile(std::ifstream& file) {
            if (!file) {
                return {};
            }
            std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            if (fileContent.empty()) {
                return {};
            }
            return fileContent;
        }
    }


    std::ostream& operator<<(std::ostream& os, const HTTPRequest& req)
    {
        std::string method{ magic_enum::enum_name(req._method) };
        os << "Method: " << method << '\n'
            << "Path: " << req._path << '\n'
            << "Version: " << req._version << '\n'
            << "Headers" << req._headers << '\n';
        using nlohmann::json;
      
        if (!req._body.empty()) {
            if (req._headers.find("Content-Type") != req._headers.end()) {
                auto h = req._headers.at("Content-Type");
                ContentType type = Utils::StrToContentType(h);
                switch (type) {
                    case ContentType::TEXT_PLAIN: {
                        std::cout << req._body << '\n';
                        break;
                    }
                    case ContentType::APPLICATION_JSON: {
                        try {
                            json j = json::parse(req._body);
                            os << "Body:\n" << j.dump(4) << '\n';
                        }
                        catch (const json::parse_error& e) {
                            std::cout << e.what() << '\n';
                        }
                    }
                }
            }
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& map) 
    {
        os << " {\n";
        for (const auto& [key, value] : map) {
            os << '\t' << key << ": " << value << '\n';
        }
        os << "}";
        return os;
    }
   
    std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& aStrings) {
        for (const auto& s : aStrings) {
            os << s << " ";
        }
        return os;
    }
}