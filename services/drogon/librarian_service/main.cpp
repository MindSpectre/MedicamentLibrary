#include <config_utils.hpp>
#include <fstream>
#include <drogon/drogon.h>

#include "db_interface_factory.hpp"
#include "librarian_service.hpp"

int main(const int argc, char *argv[]) {
    std::shared_ptr dbConnection =
        std::move(drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client(
            drug_lib::services::drogon::config_utils::create_params_from_config(
                drug_lib::services::drogon::config_utils::get_json_config(argc, argv, "params"))));

    // Load configuration and run the Drogon application
    drogon::app().registerController<drug_lib::services::drogon::Librarian>(
        std::make_shared<drug_lib::services::drogon::Librarian>(dbConnection));

    // Add a preflight (OPTIONS) handler
    drogon::app().registerPreRoutingAdvice([](const drogon::HttpRequestPtr &req, drogon::AdviceCallback &&acb, drogon::AdviceChainCallback &&accb) {
        if (req->method() == drogon::HttpMethod::Options) {
            const auto resp = drogon::HttpResponse::newHttpResponse();
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");
            resp->addHeader("Access-Control-Allow-Credentials", "true");
            resp->setStatusCode(drogon::k200OK);
            acb(resp);
        } else {
            accb();
        }
    });

    // All other responses
    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, X-Requested-With");
        resp->addHeader("Access-Control-Allow-Credentials", "true");
    });
    drogon::app().loadConfigFile(
        drug_lib::services::drogon::config_utils::get_path_config(argc, argv, "drogon_config"));
    drogon::app().run();
    return 0;
}
