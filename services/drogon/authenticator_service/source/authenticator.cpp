#include "authenticator.hpp"

void drug_lib::services::drogon::Authenticator::login(const ::drogon::HttpRequestPtr &req, std::function<void(const ::drogon::HttpResponsePtr &)> &&callback) const
{
	LOG_INFO << "Login";
	Json::Value request = *req->getJsonObject();
	if (!request)
	{
		LOG_ERROR << "Unobtainable json";
		const auto response = ::drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(::drogon::k400BadRequest);
		response->setBody("Invalid JSON payload");
		callback(response);
		return;
	}

	try
	{
		const std::string& login = request["login"].asString();
		const std::string& password = request["password"].asString();
		const bool verdict = service_.login(login, password);
		const auto response = ::drogon::HttpResponse::newHttpResponse();
		if (verdict) response->setStatusCode(::drogon::k200OK);
		else response->setStatusCode(::drogon::k400BadRequest);
		callback(response);
	}
	//TODO: Process more exceptions, like bad data or invalid login
	catch (const std::exception &e)
	{
		LOG_ERROR << "Cant signup" << e.what();
		const auto response = ::drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(::drogon::k500InternalServerError);
		response->setBody(e.what());
		callback(response);
	}
}

void drug_lib::services::drogon::Authenticator::signup(const ::drogon::HttpRequestPtr &req, std::function<void(const ::drogon::HttpResponsePtr &)> &&callback) const
{
	LOG_INFO << "Signup";
	Json::Value request = *req->getJsonObject();
	if (!request)
	{
		LOG_ERROR << "Unobtainable json";
		const auto response = ::drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(::drogon::k400BadRequest);
		response->setBody("Invalid JSON payload");
		callback(response);
		return;
	}

	try
	{
		const std::string login = request["login"].asString();
		const std::string password = request["password"].asString();
		std::optional<std::string> email = std::nullopt;
		if (request.isMember("email"))
		{
			email = request["email"].asString();
		}
		service_.signup(login, password, email);
		const auto response = ::drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(::drogon::k200OK);
		callback(response);
	}
	//TODO: Process more exceptions, like bad data or invalid login
	catch (const std::exception &e)
	{
		LOG_ERROR << "Cant signup" << e.what();
		const auto response = ::drogon::HttpResponse::newHttpResponse();
		response->setStatusCode(::drogon::k500InternalServerError);
		response->setBody(e.what());
		callback(response);
	}
}
