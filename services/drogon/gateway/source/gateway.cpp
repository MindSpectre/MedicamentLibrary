#include "gateway.hpp"
#include "gateway_utils.hpp"
// Constructor: Initialize persistent clients
drug_lib::services::drogon::Gateway::Gateway(const Json::Value &refs)
{
	LOG_DEBUG << "Gateway constructor";
	search_service_client_ = ::drogon::HttpClient::newHttpClient(refs["search_service_url"].asString());
	librarian_service_client_ = ::drogon::HttpClient::newHttpClient(refs["librarian_service_url"].asString());
}

void drug_lib::services::drogon::Gateway::hello_world(const ::drogon::HttpRequestPtr &req,
                                                      std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	Json::Value response;
	response["message"] = "Hello, World!";
	response["method"] = req->getMethodString();

	// Send the response using the callback
	const auto resp = ::drogon::HttpResponse::newHttpJsonResponse(response);
	callback(resp);
}

// Proxy to SearchService (using persistent client)
void drug_lib::services::drogon::Gateway::proxy_to_search_service(const ::drogon::HttpRequestPtr &req,
                                                                  std::function<void(const ::drogon::HttpResponsePtr &)>
                                                                  &&callback,
                                                                  const std::string &path) const
{
	LOG_INFO << "redirecting to searchService";
	// Include query parameters in the proxied path
	const auto fullPath = append_query_params(req, constants::endpoint_search_service + path);
	// Create and send the proxied request
	const auto request = ::drogon::HttpRequest::newHttpRequest();
	request->setPath(fullPath);
	request->setMethod(req->getMethod());

	search_service_client_->sendRequest(
		request, [callback](const ::drogon::ReqResult result, const ::drogon::HttpResponsePtr &resp)
		{
			if (result == ::drogon::ReqResult::Ok)
			{
				LOG_INFO << "successfully redirected";
				callback(get_body_response(resp));
			}
			else
			{
				LOG_DEBUG << "redirection failed";
				const auto errorResp = ::drogon::HttpResponse::newHttpResponse();
				errorResp->setStatusCode(::drogon::k500InternalServerError);
				errorResp->setBody("Failed to connect to SearchService");
				callback(errorResp);
			}
		});
}

// Proxy to LibrarianService (using persistent client)
void drug_lib::services::drogon::Gateway::proxy_to_librarian_service(const ::drogon::HttpRequestPtr &req,
                                                                     std::function<void(
	                                                                     const ::drogon::HttpResponsePtr &)> &&callback,
                                                                     const std::string &type, const std::string &id) const
{
	const auto fullPath = append_query_params(req, constants::endpoint_librarian_service + type + "/" + id);
	const auto request = ::drogon::HttpRequest::newHttpRequest();
	LOG_INFO << fullPath << " redirected to librarianService";
	request->setPath(fullPath);
	request->setMethod(req->getMethod());

	librarian_service_client_->sendRequest(
		request, [callback](const ::drogon::ReqResult result, const ::drogon::HttpResponsePtr &resp)
		{
			if (result == ::drogon::ReqResult::Ok)
			{
				callback(get_body_response(resp));
			}
			else
			{
				const auto errorResp = ::drogon::HttpResponse::newHttpResponse();
				errorResp->setStatusCode(::drogon::k500InternalServerError);
				errorResp->setBody("Failed to connect to LibrarianService");
				callback(errorResp);
			}
		});
}
