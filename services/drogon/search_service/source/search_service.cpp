#include "search_service.hpp"
#include "search_service_utils.hpp"
void drug_lib::services::drogon::Search::disease_search(
    const ::drogon::HttpRequestPtr& req, std::function<void(const ::drogon::HttpResponsePtr&)>&& callback)
{
    execute_search(req, std::move(callback),
                  [this, &req]
                  {
                      return service_.direct_search_diseases(
                          req->getParameter(constants::query_parameter),
                          std::stoi(req->getParameter(constants::page_number_parameter)));
                  });
}

void drug_lib::services::drogon::Search::medicament_search(
    const ::drogon::HttpRequestPtr& req, std::function<void(const ::drogon::HttpResponsePtr&)>&& callback)
{
    execute_search(req, std::move(callback),
                  [this, &req]
                  {
                      return service_.direct_search_medicaments(
                          req->getParameter(constants::query_parameter),
                          std::stoi(req->getParameter(constants::page_number_parameter)));
                  });
}

void drug_lib::services::drogon::Search::patient_search(
    const ::drogon::HttpRequestPtr& req, std::function<void(const ::drogon::HttpResponsePtr&)>&& callback)
{
    execute_search(req, std::move(callback),
                  [this, &req]
                  {
                      return service_.direct_search_patients(
                          req->getParameter(constants::query_parameter),
                          std::stoi(req->getParameter(constants::page_number_parameter)));
                  });
}

void drug_lib::services::drogon::Search::organization_search(
    const ::drogon::HttpRequestPtr& req, std::function<void(const ::drogon::HttpResponsePtr&)>&& callback)
{
    execute_search(req, std::move(callback),
                  [this, &req]
                  {
                      return service_.direct_search_organizations(
                          req->getParameter(constants::query_parameter),
                          std::stoi(req->getParameter(constants::page_number_parameter)));
                  });
}
void drug_lib::services::drogon::Search::search_through_all(
    const ::drogon::HttpRequestPtr& req, std::function<void(const ::drogon::HttpResponsePtr&)>&& callback)
{
    try
    {
        LOG_INFO << "Searching for " << req->getPath() << "...";
        LOG_INFO << "Where param is: " << req->getParameter(constants::query_parameter);
        const auto internalResult = this->service_.open_search(req->getParameter(constants::query_parameter));
        const auto response = create_json_response_from_objects(internalResult);
        callback(response);
    }
    catch (const std::exception& e)
    {
        const auto resp = ::drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(::drogon::kUnknown);
        resp->setBody(e.what());
        callback(resp);
    }
}
