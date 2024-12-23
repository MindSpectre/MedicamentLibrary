#pragma once

#include <drogon/HttpController.h>
#include "search_service_internal.hpp"
#include "search_service_utils.hpp"
namespace drug_lib::services::drogon
{
	class Search final : public ::drogon::HttpController<Search>
	{
	public:
		struct constants
		{
			static constexpr auto query_parameter = "query";
			static constexpr auto page_number_parameter = "page";
			static constexpr auto disease_search_endpoint = "/api/search/disease?query={pattern}&page={page}";
			static constexpr auto medicament_search_endpoint = "/api/search/medicament?query={pattern}&page={page}";
			static constexpr auto patient_search_endpoint = "/api/search/patient?query={pattern}&page={page}";
			static constexpr auto organization_search_endpoint = "/api/search/organization?query={pattern}&page={page}";
			static constexpr auto open_search_endpoint = "/api/search/open?query={pattern}";
		};

		METHOD_LIST_BEGIN
			ADD_METHOD_TO(Search::disease_search, constants::disease_search_endpoint, ::drogon::Get);
			ADD_METHOD_TO(Search::medicament_search, constants::medicament_search_endpoint, ::drogon::Get);
			ADD_METHOD_TO(Search::patient_search, constants::patient_search_endpoint, ::drogon::Get);
			ADD_METHOD_TO(Search::organization_search, constants::organization_search_endpoint, ::drogon::Get);
			ADD_METHOD_TO(Search::search_through_all, constants::open_search_endpoint, ::drogon::Get);
		METHOD_LIST_END

		static constexpr bool isAutoCreation = false;

		explicit Search(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			LOG_INFO << "Search service has been  created";
			set_up_db(connect);
		}

		explicit Search(std::shared_ptr<common::database::interfaces::DbInterface> &&connect)
		{
			LOG_INFO << "Search has been service created";
			set_up_db(connect);
		}

		~Search() override
		{
			LOG_INFO << "Search service has been destroyed.";
		}

		// Declare functions for endpoints

		void disease_search(const ::drogon::HttpRequestPtr &req,
		                    std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void medicament_search(const ::drogon::HttpRequestPtr &req,
		                       std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void patient_search(const ::drogon::HttpRequestPtr &req,
		                    std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void organization_search(const ::drogon::HttpRequestPtr &req,
		                         std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void search_through_all(const ::drogon::HttpRequestPtr &req,
		                        std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void set_up_db(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			LOG_INFO << "Setting up search db";
			service_.setup_from_one(connect);
		}

	private:
		template<typename Func>
		static SearchResponse handle_search(Func &&search_function)
		{
			try
			{
				return search_function();
			} catch (const common::database::exceptions::DatabaseException &db_ex)
			{
				LOG_ERROR << "Database exception: " << db_ex.what() << " " << decode_error(db_ex.get_error());
				throw std::runtime_error(db_ex.what());
			}
			catch (const std::exception &e)
			{
				LOG_ERROR << e.what();
				throw;
			}
		}



		template<typename Func>
		void execute_search(
			const ::drogon::HttpRequestPtr &req, std::function<void(const ::drogon::HttpResponsePtr &)> &&callback,
			Func &&search_function)
		{
			try
			{
				LOG_INFO << "Searching for " << req->getPath() << "...";
				LOG_INFO << "Where param is: " << req->getParameter(constants::query_parameter);
				LOG_INFO << "Where page is: " << req->getParameter(constants::page_number_parameter);
				auto internalResult = handle_search(std::forward<Func>(search_function));
				const auto response = ::drogon::HttpResponse::newHttpJsonResponse(internalResult.to_json());
				callback(response);
			} catch (const std::exception &e)
			{
				const auto resp = ::drogon::HttpResponse::newHttpResponse();
				resp->setStatusCode(::drogon::kUnknown);
				resp->setBody(e.what());
				callback(resp);
			}
		}

		SearchServiceInternal service_;
	};
} // namespace drug_lib::services::drogon
