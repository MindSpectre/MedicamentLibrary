#include <config_utils.hpp>
#include <fstream>
#include <drogon/drogon.h>

#include "db_interface_factory.hpp"
#include "search_service.hpp"

int main(const int argc, char *argv[])
{

	std::shared_ptr dbConnection =
		std::move(drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client(
			drug_lib::services::drogon::config_utils::create_params_from_config(
				drug_lib::services::drogon::config_utils::get_json_config(argc, argv, "params"))));
	// Load configuration and run the Drogon application
	drogon::app().registerController<drug_lib::services::drogon::Search>(
		std::make_shared<drug_lib::services::drogon::Search>(dbConnection));
	drogon::app().registerPostHandlingAdvice(
		[](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp)
		{
			resp->addHeader("Access-Control-Allow-Origin", "*");
		}).loadConfigFile(drug_lib::services::drogon::config_utils::get_path_config(argc, argv, "drogon_config"));


	drogon::app().run();
	return 0;
}
