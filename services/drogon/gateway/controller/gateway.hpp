#pragma once
#include <drogon/HttpController.h>
#include <drogon/HttpClient.h>
#include <memory> // For shared_ptr

#include <drogon/HttpController.h>

namespace drug_lib::services::drogon
{
	class Gateway final : public ::drogon::HttpController<Gateway>
	{
	public:
        struct constants
		{
			static constexpr auto proxy_lookup_service = "http://localhost:8001";
        	static constexpr auto proxy_librarian_service = "http://localhost:8000";
        	static constexpr std::string endpoint_lookup_service = "/search/";
        	static constexpr std::string endpoint_librarian_service = "/entity/";
        };


		METHOD_LIST_BEGIN
			ADD_METHOD_TO(Gateway::hello_world, "/hello", ::drogon::Get); // Endpoint: /hello
			ADD_METHOD_TO(Gateway::proxy_to_lookup_service, constants::endpoint_lookup_service+"{1}", ::drogon::Get);
			ADD_METHOD_TO(Gateway::proxy_to_librarian_service, constants::endpoint_librarian_service+"{1}", ::drogon::Get);
		METHOD_LIST_END

		void proxy_to_lookup_service(const ::drogon::HttpRequestPtr &req,
								  std::function<void(const::drogon:: HttpResponsePtr &)> &&callback,
								  const std::string &path) const;

		void proxy_to_librarian_service(const ::drogon::HttpRequestPtr &req,
									 std::function<void(const ::drogon::HttpResponsePtr &)> &&callback,
									 const std::string &path) const;

		static void hello_world(const ::drogon::HttpRequestPtr& req,
				 std::function<void(const ::drogon::HttpResponsePtr&)>&& callback);
		Gateway(); // Constructor to initialize persistent clients
	private:
		std::shared_ptr<::drogon::HttpClient> lookup_service_client_;
		std::shared_ptr<::drogon::HttpClient> librarian_service_client_;
	};
}