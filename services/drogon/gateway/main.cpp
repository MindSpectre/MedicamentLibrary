#include <gateway.hpp>
#include <drogon/drogon.h>
#include "config_utils.hpp"
int main(const int argc, char *argv[]) {
    // Load configuration and run the Drogon application
    auto references_to_services = drug_lib::services::drogon::config_utils::get_json_config(argc, argv, "ref");
    drogon::app().registerController<drug_lib::services::drogon::Gateway>(std::make_shared<drug_lib::services::drogon::Gateway>(references_to_services));
    drogon::app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
            //LOG_DEBUG << "postHandling1";
            resp->addHeader("Access-Control-Allow-Origin", "*");
        }).loadConfigFile(drug_lib::services::drogon::config_utils::get_path_config(argc, argv, "drogon_config")).run();
    return 0;
}
