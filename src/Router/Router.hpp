#pragma once
#include <functional>
#include <string>
#include <vector>
#include "../WebTypes.hpp"
namespace web
{
	class Router
	{
		using SplittedString = std::vector<std::string>;

	public:
		using QueryParameters = std::map<std::string, std::string>;
		using DynamicParameters = std::map<std::string, std::string>;
		using StaticHandler = std::function<HTTPResponse(const HTTPRequest &, const QueryParameters)>;
		using WildcardHandler = std::function<HTTPResponse(const HTTPRequest &, const QueryParameters, const SplittedString &)>;
		using DynamicHandler = std::function<HTTPResponse(const HTTPRequest &, const QueryParameters &, const DynamicParameters &)>;

	private:
		using ParsedPath = std::pair<SplittedString, QueryParameters>;
		struct StaticRoute
		{
			std::string _path;
			std::vector<std::string> _segments;
			StaticHandler _handler;
		};
		struct DynamicRoute
		{
			std::string _path;
			std::vector<std::string> _segments;
			DynamicHandler _handler;
		};
		struct WildcardRoute
		{
			std::string _prefix;
			std::vector<std::string> _segments;
			WildcardHandler _handler;
		};
		struct Routes
		{
			std::vector<StaticRoute> _staticRoutes;
			std::vector<DynamicRoute> _dynamicRoutes;
			std::vector<WildcardRoute> _wildcardRoutes;
		};

	public:
		Router() : _routes{} {};

		void AddStaticRoute(std::string iPath, HTTPMethod iMethod, StaticHandler iStaticHandler);
		void AddDynamicRoute(std::string iPath, HTTPMethod iMethod, DynamicHandler iDynamicHandler);
		void AddWildcardRoute(std::string iPath, HTTPMethod iMethod, WildcardHandler iWildcardHandler);
		HTTPResponse Match(const HTTPRequest &iRequest) const;

	private:
		SplittedString SplitByDelimiter(std::string iString, char iDelimeter) const;
		ParsedPath SplitPath(const std::string &iPath) const;
		std::unordered_map<HTTPMethod, Routes> _routes; // map<method, routes>
	};

}