#pragma once
#include <functional>
#include <string>
#include <vector>
#include "../WebTypes.hpp"
namespace web {
	class Router {
	public:
		using Handler = std::function<HTTPResponse(const HTTPRequest&)>;
	private:
		using QueryParameters = std::map<std::string, std::string>;
		using SplittedString = std::vector<std::string>;
		using ParsedPath = std::pair<SplittedString, QueryParameters>;
		struct Route {
			std::string _path;
			//std::vector<std::string> _segments;
			Handler _handler;
		};

		struct Routes {
			std::vector<Route> _staticRoutes;
			std::vector<Route> _dynamicRoutes;
		};

	public:
		Router() : _routes{} {};

		void AddRoute(std::string iPath, HTTPMethod iMethod, Handler iHandler);
		HTTPResponse Match(const HTTPRequest& iRequest) const;
		//void DeleteRoute(const std::string& iPath);
	private:
		SplittedString SplitByDelimiter(std::string iString, char iDelimeter) const;
		ParsedPath SplitPath(const std::string& iPath) const;
		std::unordered_map<HTTPMethod, Routes> _routes; //map<method, routes>
	};


	
}