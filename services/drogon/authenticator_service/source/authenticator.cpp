#include "authenticator.hpp"

void drug_lib::services::drogon::Authenticator::login(const ::drogon::HttpRequestPtr &req, std::function<void(const ::drogon::HttpResponsePtr &)> &&callback) const
{
	LOG_INFO << "Login";
	const auto response = ::drogon::HttpResponse::newHttpResponse();
	const std::shared_ptr<Json::Value> &request = req->getJsonObject();
	if (!request)
	{
		LOG_ERROR << "Unobtainable json";
		response->setStatusCode(::drogon::k400BadRequest);
		response->setBody("Invalid JSON payload");
		callback(response);
		return;
	}

	try
	{
		const std::string &login = (*request)["login"].asString();
		const std::string &password = (*request)["password"].asString();
		if (service_.login(login, password))
		{
			response->setStatusCode(::drogon::k200OK);
			LOG_INFO << "Login success";
		}
		else
		{
			response->setStatusCode(::drogon::k401Unauthorized);
			LOG_ERROR << "Login failed";
		}
		callback(response);
	}
	catch (const common::database::exceptions::InvalidIdentifierException &e)
	{
		if (e.get_error() == common::database::errors::db_error_code::RECORD_NOT_FOUND)
		{
			LOG_ERROR << "User has not exist. Try to signup.";
			response->setStatusCode(::drogon::k404NotFound);
			response->setBody(e.what());
			callback(response);
		}
	}
	catch (const std::exception &e)
	{
		LOG_ERROR << "Cant signup. " << e.what();
		response->setStatusCode(::drogon::k500InternalServerError);
		response->setBody(e.what());
		callback(response);
	}
}

void drug_lib::services::drogon::Authenticator::signup(const ::drogon::HttpRequestPtr &req, std::function<void(const ::drogon::HttpResponsePtr &)> &&callback) const
{
	const auto response = ::drogon::HttpResponse::newHttpResponse();
	LOG_INFO << "Signup";
	const std::shared_ptr<Json::Value> &request = req->getJsonObject();
	if (!request)
	{
		LOG_ERROR << "Unobtainable json";
		response->setStatusCode(::drogon::k400BadRequest);
		response->setBody("Invalid JSON payload");
		callback(response);
		return;
	}

	try
	{
		const std::string login = (*request)["login"].asString();
		const std::string password = (*request)["password"].asString();
		std::optional<std::string> email = std::nullopt;
		if (request->isMember("email"))
		{
			email = (*request)["email"].asString();
		}
		service_.signup(login, password, email);
		response->setStatusCode(::drogon::k200OK);
		LOG_INFO << "Successfully signed up";
		callback(response);
	}
	catch (const common::database::exceptions::InvalidIdentifierException &e)
	{
		if (e.get_error() == common::database::errors::db_error_code::DUPLICATE_RECORD)
		{
			LOG_ERROR << "User has already exist. Try to login.";
			response->setStatusCode(::drogon::k401Unauthorized);
			response->setBody(e.what());
			callback(response);
		}
	}
	catch (const std::exception &e)
	{
		LOG_ERROR << "Cant signup. " << e.what();
		response->setStatusCode(::drogon::k500InternalServerError);
		response->setBody(e.what());
		callback(response);
	}
}
