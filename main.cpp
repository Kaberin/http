#include "server.hpp"
#include "src/DefaulValues.hpp"
#include "../libs/nlohmann/json.hpp"   
#include <fstream>
#include <filesystem>
web::HTTPResponse StaticHandler1(const web::HTTPRequest& req, web::Router::QueryParameters qParams)
{
    web::HTTPResponse resp = web::DefaultValues::DEFAULT_RESPONSE;
    resp._body = "Hello from server!\nRequest body: " + req._body;

    if (!qParams.empty()) {
        resp._body += "\nQuery parameters:\n";
        for (const auto& [key, value] : qParams) {
            resp._body += "\t" + key + ": " + value + '\n';
        }
        resp._body += "\n";
    }
    return resp;
}

web::HTTPResponse DynamicHandler1(const web::HTTPRequest& req, web::Router::QueryParameters qParams, web::Router::DynamicParameters dParams)
{
    std::cout << req << '\n';

    web::HTTPResponse resp = web::DefaultValues::DEFAULT_RESPONSE;
    resp._body = "Hello from server!\nRequest body: " + req._body;

    if (!qParams.empty()) {
        resp._body += "\nQuery parameters:\n";
        for (const auto& [key, value] : qParams) {
            resp._body += "\t" + key + ": " + value + '\n';
        }
        resp._body += "\n";
    }

    if (!qParams.empty()) {
        resp._body += "\nDynamic parameters:\n";
        for (const auto& [key, value] : dParams) {
            resp._body += "\t" + key + ": " + value + '\n';
        }
        resp._body += "\n";
    }

    return resp;
}

struct user {
    std::string name;
    std::string secondName;
    int age;
    double height;
};

web::HTTPResponse UserHandler(const web::HTTPRequest& req, web::Router::QueryParameters qParams)
{
    web::HTTPResponse resp = web::DefaultValues::DEFAULT_RESPONSE;

    resp._headers[web::HeaderNames::CONTENT_TYPE] = web::HeaderValues::CONTENT_TYPE_APP_JSON;

    user u{ "Anton", "Baton", 18, 170. };
    
    nlohmann::json j{
        {"Name", u.name },
        {"SecondName", u.secondName },
        {"Age", u.age},
        {"Height", u.height}
    };

    resp._body = j.dump();
    return resp;
}

web::HTTPResponse WildHandler(const web::HTTPRequest& req, web::Router::QueryParameters qParams, const std::vector<std::string>& tail)
{
    std::cout << "\n\n========REQUEST========\n" << req << "\n========REQUESTEND========\n\n\n";

    const std::string basePath = "C:/Users/kaberin/Desktop/frontend/";
    const std::string defaultFile = "index.html";
    if (tail.size() == 0) {
        std::filesystem::path safePath = std::filesystem::path(basePath) / std::filesystem::path(defaultFile);
        std::ifstream ifs{ safePath };
        if (!ifs) {
            return web::DefaultValues::BAD_REQUEST_RESPONSE;
        }
        std::string file((std::istreambuf_iterator<char>(ifs)),
            std::istreambuf_iterator<char>());
        ifs.close();

        auto response = web::DefaultValues::DEFAULT_RESPONSE;
        response._headers[web::HeaderNames::CONTENT_TYPE] = web::HeaderValues::CONTENT_TYPE_TEXT_HTML;
        response._body = file;
        return response;
    }

    web::FileType type = web::FileType::INVALID;

    if (tail.front().ends_with(".html")) {
        type = web::FileType::HTML;
    }
    else if (tail.front().ends_with(".css")) {
        type = web::FileType::CSS;
    }
    else if (tail.front().ends_with(".js")) {
        type = web::FileType::JS;
    }

    if (type == web::FileType::INVALID) {
        return web::DefaultValues::BAD_REQUEST_RESPONSE;
    }

    std::filesystem::path safePath = std::filesystem::path(basePath) / tail.front();
    std::ifstream ifs{ safePath };
    if (!ifs) {
        return web::DefaultValues::BAD_REQUEST_RESPONSE;
    }
    std::string file((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    ifs.close();

    std::string contentType = "";

    if (type == web::FileType::HTML) {
        contentType = web::HeaderValues::CONTENT_TYPE_TEXT_HTML;
    } 
    else if (type == web::FileType::CSS) {
        contentType = web::HeaderValues::CONTENT_TYPE_TEXT_CSS;
    }
    else if (type == web::FileType::JS) {
        contentType = web::HeaderValues::CONTENT_TYPE_TEXT_JS;
    }
    else {
        return web::DefaultValues::SERVER_ERROR_RESPONSE;
    }

    auto response = web::DefaultValues::DEFAULT_RESPONSE;
    response._headers[web::HeaderNames::CONTENT_TYPE] = contentType;
    response._body = file;
    return response;
}

int main() {
#ifdef _WIN32
    web::WSAInit init;
#endif
    web::Router router;
    router.AddWildcardRoute("/webpage", web::HTTPMethod::GET, WildHandler);
    web::Server server(8080, router);
    server.Start();
    return 0;
};
