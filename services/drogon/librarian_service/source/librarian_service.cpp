#include "librarian_service.hpp"

void drug_lib::services::drogon::Librarian::get_patient(
	const ::drogon::HttpRequestPtr &,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Get patient by id: " << id.get_id();
	handle_get(callback, [&]()
	{
		return service_.get_patient(id)->to_json();
	});
}

void drug_lib::services::drogon::Librarian::update_patient(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Update patient by id: " << id.get_id();
	handle_update(req, callback, [&]()
	{
		data::objects::Patient patient;
		patient.from_json(*req->getJsonObject());
		return service_.update_patient(patient);
	});
}

void drug_lib::services::drogon::Librarian::add_patient(const ::drogon::HttpRequestPtr &req,
                                                        std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	LOG_DEBUG << "Add patient...";
	handle_add(req, callback, [&]()
	{
		data::objects::Patient patient;
		patient.from_json(*req->getJsonObject());
		service_.add_patient(patient);
		LOG_DEBUG << "Retrieved id: " << patient.get_id();
		return patient.to_json();
	});
}

void drug_lib::services::drogon::Librarian::remove_patient(
	const ::drogon::HttpRequestPtr &,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Remove patient by id: " << id.get_id();
	handle_remove(callback, [&]()
	{
		return service_.remove_patient(id);
	});
}

void drug_lib::services::drogon::Librarian::get_disease(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Get disease by id: " << id.get_id();
	handle_get(callback, [&]()
	{
		return service_.get_disease(id)->to_json();
	});
}

void drug_lib::services::drogon::Librarian::update_disease(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Update disease by id: " << id.get_id();
	handle_update(req, callback, [&]()
	{
		data::objects::Disease disease;
		disease.from_json(*(req->getJsonObject()));
		return service_.update_disease(disease);
	});
}

void drug_lib::services::drogon::Librarian::add_disease(const ::drogon::HttpRequestPtr &req,
                                                        std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	LOG_DEBUG << "Add disease...";
	handle_add(req, callback, [&]()
	{
		data::objects::Disease disease;
		disease.from_json(*req->getJsonObject());
		service_.add_disease(disease);
		LOG_DEBUG << "Retrieved id: " << disease.get_id();
		return disease.to_json();
	});
}

void drug_lib::services::drogon::Librarian::remove_disease(
	const ::drogon::HttpRequestPtr &,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Remove disease by id: " << id.get_id();
	handle_remove(callback, [&]()
	{
		return service_.remove_disease(id);
	});
}

void drug_lib::services::drogon::Librarian::get_medicament(const ::drogon::HttpRequestPtr &req,
                                                           std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Get medicament by id: " << id.get_id();
	handle_get(callback, [&]()
	{
		return service_.get_medicament(id)->to_json();
	});
}

void drug_lib::services::drogon::Librarian::update_medicament(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Update medicament by id: " << id.get_id();
	handle_update(req, callback, [&]()
	{
		data::objects::Medicament drug;
		drug.from_json(*req->getJsonObject());
		return service_.update_medicament(drug);
	});
}

void drug_lib::services::drogon::Librarian::add_medicament(const ::drogon::HttpRequestPtr &req,
                                                           std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	LOG_DEBUG << "Add medicament...";
	handle_add(req, callback, [&]()
	{
		data::objects::Medicament drug;
		drug.from_json(*req->getJsonObject());
		service_.add_medicament(drug);
		LOG_DEBUG << "Retrieved id: " << drug.get_id();
		return drug.to_json();
	});
}

void drug_lib::services::drogon::Librarian::remove_medicament(const ::drogon::HttpRequestPtr &,
                                                              std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Remove medicament by id: " << id.get_id();
	handle_remove(callback, [&]()
	{
		return service_.remove_medicament(id);
	});
}

void drug_lib::services::drogon::Librarian::get_organization(const ::drogon::HttpRequestPtr &,
                                                             std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Get organization by id: " << id.get_id();
	handle_get(callback, [&]()
	{
		return service_.get_organization(id)->to_json();
	});
}

void drug_lib::services::drogon::Librarian::update_organization(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Update organization by id: " << id.get_id();
	handle_update(req, callback, [&]()
	{
		data::objects::Organization org;
		org.from_json(*req->getJsonObject());
		return service_.update_organization(org);
	});
}

void drug_lib::services::drogon::Librarian::add_organization(const ::drogon::HttpRequestPtr &req,
                                                             std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	LOG_DEBUG << "Add organization...";
	handle_add(req, callback, [&]()
	{
		data::objects::Organization organization;
		organization.from_json(*req->getJsonObject());
		service_.add_organization(organization);
		LOG_DEBUG << "Retrieved id: " << organization.get_id();
		return organization.to_json();
	});
}

void drug_lib::services::drogon::Librarian::remove_organization(
	const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const common::database::Uuid &id)
{
	LOG_DEBUG << "Remove organization by id: " << id.get_id();
	handle_remove(callback, [&]()
	{
		return service_.remove_organization(id);
	});
}
