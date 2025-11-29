#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../DefaulValues.hpp"
#include "../Utils.hpp"
#include "../WebTypes.hpp"
#include "./Router.hpp"
#include "Logger/Logger.hpp"
namespace web {
    static bool IsHtmlCssJs(const std::string& path) {
        return (path.ends_with(".html")) || (path.ends_with(".css")) ||
               path.ends_with(".js");
    }

    static bool IsPicture(const std::string& path) {
        return (path.ends_with(".jpg")) || (path.ends_with(".png"));
    }

    static web::FileType GetFileType(const std::string& iPath) {
        if ((iPath.ends_with(".html"))) {
            return web::FileType::HTML;
        }
        if ((iPath.ends_with(".css"))) {
            return web::FileType::CSS;
        }
        if ((iPath.ends_with(".js"))) {
            return web::FileType::JS;
        }
        return web::FileType::INVALID;
    }

    web::HTTPResponse PagesHandler(const web::HTTPRequest& req,
                                   web::Router::QueryParameters qParams,
                                   const std::vector<std::string>& tail) {
        std::cout << "Request : " << req << '\n';
        std::cout << "\n\n" << req._path << "\n\n";
        std::filesystem::path basePath =
            "/mnt/c/Users/kaberin/Desktop/frontend/";
        std::filesystem::path path = req._path;
        if (!path.has_extension() && !req._path.ends_with("/")) {
            web::HTTPResponse redirect =
                web::DefaultValues::REDIRECT_RESPONSE;  // 301
            redirect._headers["Location"] = req._path + "/";
            return redirect;
        }

        if (!path.has_extension()) {
            path /= "index.html";
        }

        std::string p = path.string();
        if (!p.empty() && p[0] == '/') p.erase(0, 1);
        path = p;
        if (!IsHtmlCssJs(path.string()) && !IsPicture(path.string())) {
            return web::DefaultValues::BAD_REQUEST_RESPONSE;
        }
        std::filesystem::path safePath = basePath / path;

        std::ifstream pageFile;
        if (IsPicture(path.string())) {
            pageFile.open(safePath, std::ios::binary);
            if (!pageFile) {
                return web::DefaultValues::BAD_REQUEST_RESPONSE;
            }
            auto picture = Utils::ReadFile(pageFile);
            auto response = web::DefaultValues::DEFAULT_RESPONSE;
            response._headers[web::HeaderNames::CONTENT_TYPE] =
                web::HeaderValues::CONTENT_TYPE_IMG_JPG;
            response._body = picture;
            return response;
        }

        pageFile.open(safePath);
        if (!pageFile) {
            std::ifstream notFound{
                "C:/Users/kaberin/Desktop/frontend/404/404.html"};
            std::string fileContent = Utils::ReadFile(notFound);
            notFound.close();
            auto response = web::DefaultValues::DEFAULT_RESPONSE;
            response._headers[web::HeaderNames::CONTENT_TYPE] =
                web::HeaderValues::CONTENT_TYPE_TEXT_HTML;
            response._body = fileContent;
            return response;
        }

        std::string fileContent = Utils::ReadFile(pageFile);
        pageFile.close();

        std::string contentType = "";
        auto fileType = GetFileType(safePath.string());
        switch (fileType) {
            case web::FileType::HTML: {
                contentType = web::HeaderValues::CONTENT_TYPE_TEXT_HTML;
                break;
            }
            case web::FileType::CSS: {
                contentType = web::HeaderValues::CONTENT_TYPE_TEXT_CSS;
                break;
            }
            case web::FileType::JS: {
                contentType = web::HeaderValues::CONTENT_TYPE_TEXT_JS;
                break;
            }
            default: {
                return web::DefaultValues::SERVER_ERROR_RESPONSE;
            }
        }
        auto response = web::DefaultValues::DEFAULT_RESPONSE;
        response._headers[web::HeaderNames::CONTENT_TYPE] = contentType;
        response._body = fileContent;
        return response;
    }

    void RegisterRoutes(Router& iRouter) {
        Logger::GetInstance().Log(LogType::INFO, "Routes registration...");
        iRouter.AddWildcardRoute("/", HTTPMethod::GET, PagesHandler);
        Logger::GetInstance().Log(LogType::INFO,
                                  "Routes registration successfull!");
    }
}  // namespace web