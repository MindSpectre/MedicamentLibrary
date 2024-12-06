#include <gateway.hpp>
#include <drogon/drogon.h>
#include "config_utils.hpp"
int main(const int argc, char *argv[]) {
    // Load configuration and run the Drogon application
    auto references_to_services = drug_lib::services::drogon::config_utils::get_json_config(argc, argv, "ref");
    drogon::app().registerController<drug_lib::services::drogon::Gateway>(std::make_shared<drug_lib::services::drogon::Gateway>(references_to_services));
       drogon::app().loadConfigFile(drug_lib::services::drogon::config_utils::get_path_config(argc, argv, "drogon_config"));

    drogon::app().registerPreRoutingAdvice([](const drogon::HttpRequestPtr &req, drogon::AdviceCallback &&acb, drogon::AdviceChainCallback &&accb) {
        if (req->method() == drogon::HttpMethod::Options) {
            auto resp = drogon::HttpResponse::newHttpResponse();
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
    drogon::app().run();
    return 0;
}
