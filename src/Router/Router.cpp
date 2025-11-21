#include "Router.hpp"
#include <iostream>
#include <sstream>
#include "../DefaulValues.hpp"
namespace web {

	void Router::AddRoute(std::string iPath, HTTPMethod iMethod, Handler iHandler) {
		_routes[iMethod]._staticRoutes.push_back(Route{ iPath, iHandler });
	};

	Router::SplittedString Router::SplitByDelimiter(std::string iString, char iDelimeter) const
	{
		std::stringstream sstream{ iString };
		std::vector<std::string> segments;
		std::string segment;
		while (std::getline(sstream, segment, iDelimeter)) {
			if (segment == "") continue;
			segments.push_back(segment);
		}
		return segments;
	}

	Router::ParsedPath Router::SplitPath(const std::string& iPath) const {
		size_t p = std::string::npos;
		if ((p = iPath.find("?")) != std::string::npos && p != 0) { //for query parameters
			auto path = iPath.substr(0, p);
			auto query = iPath.substr(p + 1);
			auto pathSegments = SplitByDelimiter(path, '/');
			auto queryParams = SplitByDelimiter(query, '&');
			QueryParameters params;
			//if (!params.empty()) {
				for (auto& param : queryParams) {
					auto splitedParams = SplitByDelimiter(param, '=');
					params[splitedParams.at(0)] = splitedParams.at(1);
				}
			//}
			return { pathSegments, params };
		}
		else {
			auto pathSegments = SplitByDelimiter(iPath, '/');
			return { pathSegments, {} };
		}
		return {};
	}

	HTTPResponse Router::Match(const HTTPRequest& iRequest) const
	{
		auto iPath = iRequest._path;
		auto iMethod = iRequest._method;

		auto [aPathSegments, mQueryParameters] = SplitPath(iPath);

		std::string path;
		for (size_t i = 0; i < aPathSegments.size(); ++i) {
			if (!aPathSegments[i].empty()) {
				if (!path.empty()) path += '/';
				path += aPathSegments[i];
			}
		}
		if (_routes.find(iMethod) == _routes.end()) {
			return DEFAULT_VALUES::NOT_ALLOWED_RESPONSE;
		}
		const auto& routes = _routes.at(iMethod);
		const auto& staticRoutes = routes._staticRoutes;

		for (auto& route : staticRoutes) {
			if (route._path == path) {
				HTTPResponse response = route._handler(iRequest);
				return response;
			}
		}

		return DEFAULT_VALUES::NOT_ALLOWED_RESPONSE;
	}
}
