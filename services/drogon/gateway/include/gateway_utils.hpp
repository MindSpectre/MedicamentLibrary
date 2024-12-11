#pragma once

#include <string>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

namespace drug_lib::services::drogon
{
	// Utility to convert strings to integers with error handling
	inline std::string append_query_params(const ::drogon::HttpRequestPtr &req, const std::string &path)
	{
		const auto &queryParams = req->getParameters();
		if (queryParams.empty())
		{
			return path; // No query parameters, return the original path
		}

		// Build the query string
		std::string fullPath = path + "?";
		for (const auto &[fst, snd]: queryParams)
		{
			fullPath.append(fst).append("=").append(snd).append("&");
		}
		fullPath.pop_back(); // Remove the trailing '&'
		return fullPath;
	}

	inline ::drogon::HttpResponsePtr get_body_response(const ::drogon::HttpResponsePtr &response)
	{
		auto newResponse = ::drogon::HttpResponse::newHttpResponse();
		newResponse->setBody(std::string(response->body()));
		return newResponse;
	}
}
