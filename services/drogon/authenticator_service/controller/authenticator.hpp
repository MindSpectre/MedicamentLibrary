#pragma once
#include <authenticator_service_internal.hpp>
#include <memory> // For shared_ptr
#include <drogon/HttpClient.h>
#include <drogon/HttpController.h>


namespace drug_lib::common::database::interfaces
{
	class DbInterface;
}

namespace drug_lib::services::drogon
{
	struct constants
	{
		static constexpr char endpoint_login[] = "/api/auth/login";
		static constexpr char endpoint_signup[] = "/api/auth/signup";
	};

	class Authenticator final : public ::drogon::HttpController<Authenticator>
	{
	public:
		METHOD_LIST_BEGIN
			ADD_METHOD_TO(Authenticator::login, constants::endpoint_login, ::drogon::Post);
			ADD_METHOD_TO(Authenticator::signup, constants::endpoint_signup, ::drogon::Post);
		METHOD_LIST_END

		static constexpr bool isAutoCreation = false;

		explicit Authenticator(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			LOG_INFO << "Authenticator service has been created";
			this->set_up_db(connect);
		}

		~Authenticator() override
		{
			LOG_INFO << "Authenticator has been destroyed";
		}

	private:
		AuthenticatorServiceInternal service_;

		void login(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback) const;

		void signup(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback) const;

		void set_up_db(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			LOG_INFO << "Setting up auth db";
			service_.setup_from_one(connect);
		}
	};
}
