#pragma once

#include <drogon/HttpController.h>

#include "compile_time_utils.hpp"
#include "librarian_service_internal.hpp"

namespace drug_lib::services::drogon
{
	class Librarian final : public ::drogon::HttpController<Librarian>
	{
	public:
		struct constants
		{
			static constexpr auto query_parameter = "query";
			static constexpr auto disease_wiki_endpoint = "/api/wiki/disease/";
			static constexpr auto disease_wiki_endpoint_n = "/api/wiki/disease/{1}";
			static constexpr auto medicament_wiki_endpoint = "/api/wiki/medicament/";
			static constexpr auto medicament_wiki_endpoint_n = "/api/wiki/medicament/{1}";
			static constexpr auto patient_wiki_endpoint = "/api/wiki/patient/";
			static constexpr auto patient_wiki_endpoint_n = "/api/wiki/patient/{1}";
			static constexpr auto organization_wiki_endpoint = "/api/wiki/organization/";
			static constexpr auto organization_wiki_endpoint_n = "/api/wiki/organization/{1}";
		};

		METHOD_LIST_BEGIN
			ADD_METHOD_TO(Librarian::get_patient, constants::patient_wiki_endpoint_n, ::drogon::Get);
			ADD_METHOD_TO(Librarian::update_patient, constants::patient_wiki_endpoint_n, ::drogon::Put);
			ADD_METHOD_TO(Librarian::add_patient, constants::patient_wiki_endpoint, ::drogon::Post);
			ADD_METHOD_TO(Librarian::remove_patient, constants::patient_wiki_endpoint_n, ::drogon::Delete);

			ADD_METHOD_TO(Librarian::get_disease, constants::disease_wiki_endpoint_n, ::drogon::Get);
			ADD_METHOD_TO(Librarian::update_disease, constants::disease_wiki_endpoint_n, ::drogon::Put);
			ADD_METHOD_TO(Librarian::add_disease, constants::disease_wiki_endpoint, ::drogon::Post);
			ADD_METHOD_TO(Librarian::remove_disease, constants::disease_wiki_endpoint_n, ::drogon::Delete);

			ADD_METHOD_TO(Librarian::get_medicament, constants::medicament_wiki_endpoint_n, ::drogon::Get);
			ADD_METHOD_TO(Librarian::update_medicament, constants::medicament_wiki_endpoint_n, ::drogon::Put);
			ADD_METHOD_TO(Librarian::add_medicament, constants::medicament_wiki_endpoint, ::drogon::Post);
			ADD_METHOD_TO(Librarian::remove_medicament, constants::medicament_wiki_endpoint_n, ::drogon::Delete);

			ADD_METHOD_TO(Librarian::get_organization, constants::organization_wiki_endpoint_n, ::drogon::Get);
			ADD_METHOD_TO(Librarian::update_organization, constants::organization_wiki_endpoint_n, ::drogon::Put);
			ADD_METHOD_TO(Librarian::add_organization, constants::organization_wiki_endpoint, ::drogon::Post);
			ADD_METHOD_TO(Librarian::remove_organization, constants::organization_wiki_endpoint_n, ::drogon::Delete);
		METHOD_LIST_END

		explicit Librarian(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			LOG_INFO << "Librarian service has been created";
			this->set_up_db(connect);
		}

		explicit Librarian(std::shared_ptr<common::database::interfaces::DbInterface> &&connect)
		{
			LOG_INFO << "Librarian service has been created";
			this->set_up_db(std::move(connect));
		}

		static constexpr bool isAutoCreation = false;

		~Librarian() override
		{
			LOG_INFO << "Librarian service has been destroyed";
		}

	private:
		void set_up_db(const std::shared_ptr<common::database::interfaces::DbInterface> &connect)
		{
			LOG_INFO << "Setting up search db";
			service_.setup_from_one(connect);
		}

		LibrarianServiceInternal service_;

		// Patient Methods
		void get_patient(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void update_patient(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void add_patient(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void remove_patient(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		// Disease Methods
		void get_disease(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void update_disease(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void add_disease(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void remove_disease(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void get_medicament(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void update_medicament(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void add_medicament(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void remove_medicament(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);


		void get_organization(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void update_organization(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		void add_organization(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback);

		void remove_organization(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, common::database::Uuid id);

		// Shared Handlers for CRUD Operations
		static void handle_get(const std::function<void(const ::drogon::HttpResponsePtr &)> &callback, const std::function<Json::Value()> &get_func)
		{
			LOG_INFO << "Get element";
			try
			{
				Json::Value wiki = get_func();
				const auto response = ::drogon::HttpResponse::newHttpJsonResponse(std::move(wiki));
				response->setStatusCode(::drogon::k200OK);
				callback(response);
			}
			catch (const std::exception &e)
			{
				LOG_ERROR << "Caught exception: " << e.what();
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k404NotFound);
				response->setBody(e.what());
				callback(response);
			}
		}

		template <typename UpdateFunction>
		static void handle_update(
			const ::drogon::HttpRequestPtr &req,
			std::function<void(const ::drogon::HttpResponsePtr &)> &callback, UpdateFunction update_func)
		{
			LOG_INFO << "Update element";
			if (const auto json = req->getJsonObject();
				!json)
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
				update_func();
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k200OK);
				callback(response);
			}
			catch (const std::exception &e)
			{
				LOG_ERROR << "Error During conversion" << e.what();
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k500InternalServerError);
				response->setBody(e.what());
				callback(response);
			}
		}

		static void handle_add(
			const ::drogon::HttpRequestPtr &req,
			const std::function<void(const ::drogon::HttpResponsePtr &)> &callback, const std::function<Json::Value()> &add_func)
		{
			LOG_INFO << "Add element";
			if (const auto json = req->getJsonObject();
				!json)
			{
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k400BadRequest);
				response->setBody("Invalid JSON payload");
				callback(response);
				return;
			}

			try
			{
				Json::Value fetched = add_func();
				const auto response = ::drogon::HttpResponse::newHttpJsonResponse(std::move(fetched));
				response->setStatusCode(::drogon::k201Created);
				callback(response);
			}
			catch (const std::exception &e)
			{
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k500InternalServerError);
				response->setBody(e.what());
				callback(response);
			}
		}

		template <typename RemoveFunction>
		static void handle_remove(std::function<void(const ::drogon::HttpResponsePtr &)> &callback, RemoveFunction remove_func)
		{
			LOG_INFO << "Remove element";
			try
			{
				remove_func();
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k204NoContent);
				callback(response);
			}
			catch (const std::exception &e)
			{
				const auto response = ::drogon::HttpResponse::newHttpResponse();
				response->setStatusCode(::drogon::k500InternalServerError);
				response->setBody(e.what());
				callback(response);
			}
		}
	};
} // namespace drug_lib::services::::drogon
