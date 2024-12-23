#include "gateway.hpp"
#include "gateway_utils.hpp"
#include "stopwatch.hpp"
// Constructor: Initialize persistent clients
drug_lib::services::drogon::Gateway::Gateway(const Json::Value &refs)
{
	LOG_INFO << "Gateway has been created in " << refs["name"].asString() << " space.";
	search_service_client_ = ::drogon::HttpClient::newHttpClient(refs["search_service_url"].asString());
	librarian_service_client_ = ::drogon::HttpClient::newHttpClient(refs["librarian_service_url"].asString());
	authenticator_service_client_ = ::drogon::HttpClient::newHttpClient(refs["authenticator_service_url"].asString());
	LOG_DEBUG << "Services has been connected to network.";
	LOG_DEBUG << "Search service URL: " << search_service_client_->getHost().c_str() << ":" << search_service_client_->getPort();
	LOG_DEBUG << "Library service URL: " << librarian_service_client_->getHost().c_str() << ":" << librarian_service_client_->getPort();
	LOG_DEBUG << "Authenticator service URL: " << authenticator_service_client_->getHost().c_str() <<":" << authenticator_service_client_->getPort();
}

void drug_lib::services::drogon::Gateway::hello_world(
	const ::drogon::HttpRequestPtr &req,
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
void drug_lib::services::drogon::Gateway::proxy_to_search_service(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)>
	&&callback,
	const std::string &path) const
{
	// Include query parameters in the proxied path
	const auto full_path = append_query_params(req, constants::endpoint_search_service + path);
	common::Stopwatch sw(" Redirected to Search Service with path" + full_path);
	// Create and send the proxied request
	const auto request = ::drogon::HttpRequest::newHttpRequest();
	request->setPath(full_path);
	request->setMethod(req->getMethod());
	sw.start();
	search_service_client_->sendRequest(
		request, [callback](const ::drogon::ReqResult result, const ::drogon::HttpResponsePtr &resp)
		{
			if (result == ::drogon::ReqResult::Ok)
			{
				LOG_INFO << "Redirection passed";
				const auto external_resp = get_body_response(resp);
				external_resp->setStatusCode(resp->getStatusCode());
				callback(external_resp);
			}
			else
			{
				LOG_ERROR << "Redirection failed";
				const auto error_response = ::drogon::HttpResponse::newHttpResponse();
				error_response->setStatusCode(::drogon::k500InternalServerError);
				error_response->setBody("Failed to connect to Search Service");
				callback(error_response);
			}
		});
	sw.finish();
}

// Proxy to LibrarianService (using persistent client)
void drug_lib::services::drogon::Gateway::proxy_to_librarian_service(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(
		const ::drogon::HttpResponsePtr &)> &&callback,
	const std::string &type, const std::string &id) const
{
	const auto full_path = append_query_params(req, constants::endpoint_librarian_service + type + "/" + id);
	common::Stopwatch<std::chrono::nanoseconds> sw(" Redirected to Librarian Service with path" + full_path);

	const auto cloned_request = ::drogon::HttpRequest::newHttpRequest();
	cloned_request->setPath(full_path);
	req->setPath(full_path);
	req->setContentTypeCode(::drogon::ContentType::CT_APPLICATION_JSON);
	sw.start();
	librarian_service_client_->sendRequest(
		req, [callback](const ::drogon::ReqResult result, const ::drogon::HttpResponsePtr &resp)
		{
			if (result == ::drogon::ReqResult::Ok)
			{
				LOG_INFO << "Redirection passed";
				const auto external_resp = get_body_response(resp);
				external_resp->setStatusCode(resp->getStatusCode());
				callback(external_resp);
			}
			else
			{
				LOG_ERROR << "Redirection failed";
				const auto error_response = ::drogon::HttpResponse::newHttpResponse();
				error_response->setStatusCode(::drogon::k500InternalServerError);
				error_response->setBody("Failed to connect to Librarian Service");
				callback(error_response);
			}
		});
	sw.finish();
}

void drug_lib::services::drogon::Gateway::proxy_to_authenticator_service(
	const ::drogon::HttpRequestPtr &req, std::function<void(const ::drogon::HttpResponsePtr &)> &&callback,
	const std::string &path) const
{
	const auto full_path = append_query_params(req, constants::endpoint_authenticator_service + path);
	common::Stopwatch<std::chrono::nanoseconds> sw(" Redirected to Authenticator Service with path" + full_path);

	const auto cloned_request = ::drogon::HttpRequest::newHttpRequest();
	cloned_request->setPath(full_path);
	req->setPath(full_path);
	req->setContentTypeCode(::drogon::ContentType::CT_APPLICATION_JSON);
	sw.start();
	authenticator_service_client_->sendRequest(
		req, [callback](const ::drogon::ReqResult result, const ::drogon::HttpResponsePtr &resp)
		{
			if (result == ::drogon::ReqResult::Ok)
			{
				LOG_INFO << "Redirection passed";
				const auto external_resp = get_body_response(resp);
				external_resp->setStatusCode(resp->getStatusCode());
				callback(external_resp);
			}
			else
			{
				LOG_ERROR << "Redirection failed";
				const auto error_response = ::drogon::HttpResponse::newHttpResponse();
				error_response->setStatusCode(::drogon::k500InternalServerError);
				error_response->setBody("Failed to connect to Authenticator Service");
				callback(error_response);
			}
		});
	sw.finish();
}
