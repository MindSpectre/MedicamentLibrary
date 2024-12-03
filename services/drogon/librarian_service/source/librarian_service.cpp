#include "librarian_service.hpp"

void drug_lib::services::drogon::Librarian::get_patient(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	handle_get(callback, [&]()
	{
		return service_.get_patient(id);
	});
}

void drug_lib::services::drogon::Librarian::update_patient(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, int32_t id)
{
	handle_update(req, callback, [&]()
	{
		data::objects::Patient patient;
		patient.from_json(req->getJsonObject().get());
		return service_.update_patient(patient);
	});
}

void drug_lib::services::drogon::Librarian::add_patient(const ::drogon::HttpRequestPtr &req,
                                                        std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	handle_add(req, callback, [&]()
	{
		data::objects::Patient patient;
		patient.from_json(req->getJsonObject().get());
		return service_.add_patient(patient);
	});
}

void drug_lib::services::drogon::Librarian::remove_patient(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	handle_remove(callback, [&]()
	{
		return service_.remove_patient(id);
	});
}

void drug_lib::services::drogon::Librarian::get_disease(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	LOG_DEBUG << "E";
	handle_get(callback, [&]()
	{
		return service_.get_disease(id);
	});
}

void drug_lib::services::drogon::Librarian::update_disease(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, int32_t id)
{
	handle_update(req, callback, [&]()
	{
		data::objects::Disease disease;
		disease.from_json(req->getJsonObject().get());
		return service_.update_disease(disease);
	});
}

void drug_lib::services::drogon::Librarian::add_disease(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	handle_add(req, callback, [&]()
	{
		data::objects::Disease disease;
		disease.from_json(req->getJsonObject().get());
		return service_.add_disease(disease);
	});
}

void drug_lib::services::drogon::Librarian::remove_disease(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, int32_t id)
{
	handle_remove(callback, [&]()
	{
		return service_.remove_disease(id);
	});
}

void drug_lib::services::drogon::Librarian::get_medicament(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	handle_get(callback, [&]()
	{
		return service_.get_medicament(id);
	});
}

void drug_lib::services::drogon::Librarian::update_medicament(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, int32_t id)
{
	handle_update(req, callback, [&]()
	{
		data::objects::Medicament drug;
		drug.from_json(req->getJsonObject().get());
		return service_.update_medicament(drug);
	});
}

void drug_lib::services::drogon::Librarian::add_medicament(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	handle_add(req, callback, [&]()
	{
		data::objects::Medicament drug;
		drug.from_json(req->getJsonObject().get());
		return service_.add_medicament(drug);
	});
}

void drug_lib::services::drogon::Librarian::remove_medicament(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	handle_remove(callback, [&]()
	{
		return service_.remove_medicament(id);
	});
}

void drug_lib::services::drogon::Librarian::get_organization(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	handle_get(callback, [&]()
	{
		return service_.get_organization(id);
	});
}

void drug_lib::services::drogon::Librarian::update_organization(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, int32_t id)
{
	handle_update(req, callback, [&]()
	{
		data::objects::Organization org;
		org.from_json(req->getJsonObject().get());
		return service_.update_organization(org);
	});
}

void drug_lib::services::drogon::Librarian::add_organization(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback)
{
	handle_add(req, callback, [&]()
	{
		data::objects::Organization organization;
		organization.from_json(req->getJsonObject().get());
		return service_.add_organization(organization);
	});
}

void drug_lib::services::drogon::Librarian::remove_organization(const ::drogon::HttpRequestPtr &req,
	std::function<void(const ::drogon::HttpResponsePtr &)> &&callback, const int32_t id)
{
	handle_remove(callback, [&]()
	{
		return service_.remove_organization(id);
	});
}
