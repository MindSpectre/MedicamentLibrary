#pragma once

#include <fstream>
#include <json/json.h>
#include <trantor/utils/Logger.h>

#include "db_interface_factory.hpp"
namespace drug_lib::services::drogon::config_utils
{
	inline common::database::PqxxConnectParams create_params_from_config(const Json::Value &json)
	{
		drug_lib::common::database::PqxxConnectParams params;
		params.set_host(json["host"].asString());
		params.set_port(json["port"].asInt());
		params.set_db_name(json["db_name"].asString());
		params.set_login(json["login"].asString());
		params.set_password(json["password"].asString());
		return params;
	}

	inline std::string serve_path_for_config(const int argc, char *argv[])
	{
		const auto local_config_file = "./config/local/";

		// Check the number of arguments
		if (argc == 1)
		{
			// No arguments provided, use default behavior
			return local_config_file;
		}
		if (argc == 2)
		{
			LOG_INFO << "GOT HOE";
			// One argument provided, check its value
			if (const std::string_view arg = argv[1];
				arg == "--local")
			{
				return local_config_file;
			}
			else
			{
				if (arg == "--docker")
				{
					const auto docker_config_file = "./config/docker/";
					return docker_config_file;
				}
				throw std::invalid_argument("Invalid argument");
			}
		}
		throw std::invalid_argument("Invalid argument");
	}
	inline std::string ensure_json_extension(const std::string& filename) {
		// Check if the filename ends with ".json"
		if (filename.size() >= 5 && filename.substr(filename.size() - 5) == ".json") {
			return filename; // Already has ".json"
		}
		// Append ".json" if not present
		return filename + ".json";
	}
	inline Json::Value get_json_config( const int argc, char *argv[], const std::string& name)
	{
		// Path to the JSON file
		const std::string filePath = serve_path_for_config(argc, argv) + ensure_json_extension(name);
		std::ifstream file(filePath, std::ifstream::binary);
		if (!file.is_open())
		{
			std::cerr << "Error opening file: " << filePath << std::endl;
			return 1;
		}
		Json::Value root;
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;
		std::string errs;
		if (!parseFromStream(builder, file, &root, &errs))
		{
			std::cerr << "Error parsing JSON: " << errs << std::endl;
			return 1;
		}
		file.close();
		return root;
	}
	inline std::string get_path_config( const int argc, char *argv[], const std::string& name)
	{
		// Path to the JSON file
		return serve_path_for_config(argc, argv) + ensure_json_extension(name);
	}
}