#include "Router.hpp"
#include <iostream>
#include <sstream>
#include <concepts>
#include "../DefaulValues.hpp"
namespace web
{

	void Router::AddStaticRoute(std::string iPath, HTTPMethod iMethod, StaticHandler iStaticHandler)
	{
		auto pathParts = SplitByDelimiter(iPath, '/');
		_routes[iMethod]._staticRoutes.push_back(StaticRoute{iPath, pathParts, iStaticHandler});
	};

	void Router::AddDynamicRoute(std::string iPath, HTTPMethod iMethod, DynamicHandler iDynamicHandler)
	{
		auto pathParts = SplitByDelimiter(iPath, '/');
		_routes[iMethod]._dynamicRoutes.push_back(DynamicRoute{iPath, pathParts, iDynamicHandler});
	}

	void Router::AddWildcardRoute(std::string iPath, HTTPMethod iMethod, WildcardHandler iWildcardHandler)
	{
		auto pathParts = SplitByDelimiter(iPath, '/');
		_routes[iMethod]._wildcardRoutes.push_back(WildcardRoute{iPath, pathParts, iWildcardHandler});
	}

	Router::SplittedString Router::SplitByDelimiter(std::string iString, char iDelimeter) const
	{
		std::stringstream sstream{iString};
		std::vector<std::string> segments;
		std::string segment;
		while (std::getline(sstream, segment, iDelimeter))
		{
			if (segment == "")
				continue;
			segments.push_back(segment);
		}
		return segments;
	}

	Router::ParsedPath Router::SplitPath(const std::string &iPath) const
	{
		size_t p = std::string::npos;
		if ((p = iPath.find("?")) != std::string::npos && p != 0)
		{ // for query parameters
			auto path = iPath.substr(0, p);
			auto query = iPath.substr(p + 1);
			auto pathSegments = SplitByDelimiter(path, '/');
			auto queryParams = SplitByDelimiter(query, '&');
			QueryParameters params;
			for (auto &param : queryParams)
			{
				auto splitedParams = SplitByDelimiter(param, '=');
				if (splitedParams.size() == 2)
				{
					params[splitedParams.at(0)] = splitedParams.at(1);
				}
			}
			return {pathSegments, params};
		}
		else
		{
			auto pathSegments = SplitByDelimiter(iPath, '/');
			return {pathSegments, {}};
		}
		return {};
	}

	template <class T>
	concept HasSize = requires(const T &t) {
		{ t.size() } -> std::convertible_to<size_t>;
		{ t.begin() };
		{ t.end() };
		{ t[0] };
	};

	template <HasSize T>
	static bool VecStartsWith(const T &iVecToCheck, const T &iVec, T &iTail) // checks if vec1 starts with vec2
	{
		if (iVec.size() > iVecToCheck.size())
		{
			return false;
		}

		for (int i = 0; i < iVec.size(); ++i)
		{
			if (iVec[i] != iVecToCheck[i])
			{
				return false;
			}
		}
		iTail = T{iVecToCheck.begin() + iVec.size(), iVecToCheck.end()};
		return true;
	}

	HTTPResponse Router::Match(const HTTPRequest &iRequest) const
	{
		auto iPath = iRequest._path;
		auto iMethod = iRequest._method;

		auto [aPathSegments, mQueryParameters] = SplitPath(iPath);

		if (_routes.find(iMethod) == _routes.end())
		{
			return DefaultValues::NOT_ALLOWED_RESPONSE;
		}
		const auto &routes = _routes.at(iMethod);
		const auto &staticRoutes = routes._staticRoutes;

		for (auto &route : staticRoutes)
		{
			if (route._segments == aPathSegments)
			{
				HTTPResponse response = route._handler(iRequest, mQueryParameters);
				return response;
			}
		}

		const auto &dynamicRoutes = routes._dynamicRoutes;

		for (const auto &route : dynamicRoutes)
		{
			const auto &dynamicPathSegments = route._segments;
			if (dynamicPathSegments.size() != aPathSegments.size())
			{
				continue;
			}

			std::map<std::string, std::string> mDynamicParams;
			bool matched = true;
			for (int i = 0; i < dynamicPathSegments.size(); ++i)
			{
				if (dynamicPathSegments[i].at(0) == ':')
				{ // dynamic part
					auto &dynamicParam = aPathSegments[i];
					mDynamicParams.insert({dynamicPathSegments[i].substr(1), dynamicParam});
				}
				else if (dynamicPathSegments[i] != aPathSegments[i])
				{
					matched = false;
					break;
				}
			}

			if (matched)
			{
				HTTPResponse response = route._handler(iRequest, mQueryParameters, mDynamicParams);
				return response;
			}
		}

		const auto &wildcardRoutes = routes._wildcardRoutes;

		for (const auto &route : wildcardRoutes)
		{
			std::vector<std::string> tail;
			if (VecStartsWith(aPathSegments, route._segments, tail))
			{
				return route._handler(iRequest, mQueryParameters, tail);
			}
		}

		return DefaultValues::NOT_ALLOWED_RESPONSE;
	}
}
