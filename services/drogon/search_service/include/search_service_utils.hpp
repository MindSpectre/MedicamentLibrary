#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>

namespace drug_lib::services::drogon
{
	inline std::shared_ptr<::drogon::HttpResponse> create_json_response_from_objects(
				const std::vector<std::unique_ptr<data::objects::ObjectBase> > &objects)
	{
		Json::Value response(Json::arrayValue);
		for (const auto &object: objects)
		{
			response.append(object->to_json());
		}
		return ::drogon::HttpResponse::newHttpJsonResponse(response);
	}
}
