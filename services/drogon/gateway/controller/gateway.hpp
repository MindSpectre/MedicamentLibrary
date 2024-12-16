#pragma once
#include <memory> // For shared_ptr
#include <drogon/HttpClient.h>
#include <drogon/HttpController.h>

namespace drug_lib::services::drogon
{
	struct constants
	{
		static constexpr std::string endpoint_search_service = "/api/search/";
		static constexpr std::string endpoint_librarian_service = "/api/wiki/";
	};

	class Gateway final : public ::drogon::HttpController<Gateway>
	{
	public:
		METHOD_LIST_BEGIN
			ADD_METHOD_TO(Gateway::hello_world, "/hello", ::drogon::Get); // Endpoint: /hello
			ADD_METHOD_TO(Gateway::proxy_to_search_service, constants::endpoint_search_service+"{1}", ::drogon::Get);
			ADD_METHOD_TO(Gateway::proxy_to_librarian_service, constants::endpoint_librarian_service+"{1}/{2}",
			              ::drogon::Get,
			              ::drogon::Post,
			              ::drogon::Put,
			              ::drogon::Delete,
			              ::drogon::Options);
		METHOD_LIST_END

		static constexpr bool isAutoCreation = false;

		explicit Gateway(const Json::Value &refs); // Constructor to initialize persistent clients
		~Gateway() override
		{
			LOG_INFO << "Gateway has been destroyed";
		}

	private:
		void proxy_to_search_service(const ::drogon::HttpRequestPtr &req,
		                             std::function<void(const ::drogon::HttpResponsePtr &)> &&callback,
		                             const std::string &path) const;

		void proxy_to_librarian_service(const ::drogon::HttpRequestPtr &req,
		                                std::function<void(const ::drogon::HttpResponsePtr &)> &&callback,
		                                const std::string &type, const std::string &id) const;

		static void hello_world(const ::drogon::HttpRequestPtr &req,
		                        std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		std::shared_ptr<::drogon::HttpClient> search_service_client_;
		std::shared_ptr<::drogon::HttpClient> librarian_service_client_;
	};
}
