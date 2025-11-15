#include "Utils.hpp"
#include "libs/nlohmann/json.hpp"
#include <algorithm>
namespace MyHTTP {
    namespace Utils {
        int StringToInteger(const std::string& iString) {
            std::cout << "String: " << iString << '\n';
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
            json j = json::parse(req._body);
            os << "Body:\n" << j.dump(4) << '\n';
        }

        return os;
    }

    std::ostream& operator<<(std::ostream& os, const std::map<std::string, std::string>& map) {
        os << " {\n";
        for (const auto& [key, value] : map) {
            os << '\t' << key << ": " << value << '\n';
        }
        os << "}";
        return os;
    }
}