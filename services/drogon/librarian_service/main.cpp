#include <fstream>
#include <drogon/drogon.h>

#include "db_interface_factory.hpp"
#include "librarian_service.hpp"

drug_lib::common::database::PqxxConnectParams create_params_from_config(const Json::Value& json)
{
    drug_lib::common::database::PqxxConnectParams params;
    params.set_host(json["host"].asString());
    params.set_port(json["port"].asInt());
    params.set_db_name(json["db_name"].asString());
    params.set_login(json["login"].asString());
    params.set_password(json["password"].asString());
    return params;
}
int main() {
    // Path to the JSON file
    const std::string filePath = "./config/params.json";
    std::ifstream file(filePath, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return 1;
    }
    Json::Value root;
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    std::string errs;
    if (!parseFromStream(builder, file, &root, &errs)) {
        std::cerr << "Error parsing JSON: " << errs << std::endl;
        return 1;
    }
    file.close();
    auto pm = create_params_from_config(root);
    LOG_DEBUG << pm.make_connect_string();
    std::shared_ptr<drug_lib::common::database::interfaces::DbInterface> dbConnection =
        std::move(drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client(pm));
    // Load configuration and run the Drogon application
    drogon::app().registerController<drug_lib::services::drogon::Librarian>(std::make_shared<drug_lib::services::drogon::Librarian>(dbConnection));
    drogon::app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp) {
            //LOG_DEBUG << "postHandling1";
            resp->addHeader("Access-Control-Allow-Origin", "*");
        }).loadConfigFile("./config/drogon_config.json");



    drogon::app().run();
    return 0;
}
