import os
import argparse


def create_service_structure(base_path, service_name, cmake_target, cpp_name):
    # Define folder structure
    folders = [
        f"{service_name}/config/local",
        f"{service_name}/config/docker",
        f"{service_name}/controller",
        f"{service_name}/include/f_http",
        f"{service_name}/source"
    ]

    files = {
        f"{service_name}/Dockerfile": f"""
FROM ubuntu:latest

WORKDIR /app

COPY build-release/services/drogon/{service_name}/ .

EXPOSE 8000
ENTRYPOINT ["./{cmake_target}", "--docker"]
""",
        f"{service_name}/CMakeLists.txt": f"""
add_executable({cmake_target}
        main.cpp
        source/{service_name}.cpp
)

target_include_directories({cmake_target}
        PRIVATE
        controller
        include
)

target_link_libraries({cmake_target}
        PRIVATE
        ${{NecessaryDrogonLibs}}
)

file(COPY config DESTINATION ${{CMAKE_CURRENT_BINARY_DIR}})
""",
        f"{service_name}/main.cpp": f"""
#include <config_utils.hpp>
#include <fstream>
#include <drogon/drogon.h>

#include "db_interface_factory.hpp"
#include "{service_name}.hpp"

int main(const int argc, char *argv[]) {{
    std::shared_ptr dbConnection =
        std::move(drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client(
            drug_lib::services::drogon::config_utils::create_params_from_config(
                drug_lib::services::drogon::config_utils::get_json_config(argc, argv, "params"))));

    drogon::app().registerController<drug_lib::services::drogon::{cpp_name}>(
        std::make_shared<drug_lib::services::drogon::{cpp_name}>());

    drogon::app().loadConfigFile(
        drug_lib::services::drogon::config_utils::get_path_config(argc, argv, "drogon_config"));
    drogon::app().run();
    return 0;
}}
""",
        f"{service_name}/source/{service_name}.cpp": f"""
#include "{service_name}.hpp"
""",
        f"{service_name}/controller/{service_name}.hpp": f"""
#pragma once

#include <drogon/HttpController.h>

namespace drug_lib::services::drogon {{
	class {cpp_name} final : public ::drogon::HttpController<{cpp_name}> {{
	public:
	    static constexpr bool isAutoCreation = false;
		{cpp_name}() {{ LOG_INFO << "{cpp_name} service has been created"; }}
		~{cpp_name}() override {{ LOG_INFO << "{cpp_name} service has been destroyed"; }}
	}};
}} // namespace drug_lib::services::drogon
""",
        f"{service_name}/config/local/.env": "POSTGRES_USER=postgres\nPOSTGRES_PASSWORD=postgres\nPOSTGRES_DB=test_db\n",
        f"{service_name}/config/local/drogon_config.json": """
{
  "listeners": [
    {
      "address": "0.0.0.0",
      "port": 8000
    }
  ],
  "app": {
    "threads_num": 4,
    "enable_cross_origin_resource_sharing": true
  }
}
""",
        f"{service_name}/config/local/params.json": """
{
  "host": "postgres",
  "port": 5432,
  "db_name": "test_db",
  "login": "postgres",
  "password": "postgres"
}
"""
    }

    # Create folders
    for folder in folders:
        os.makedirs(os.path.join(base_path, folder), exist_ok=True)

    # Create files
    for file_path, content in files.items():
        full_path = os.path.join(base_path, file_path)
        with open(full_path, "w") as f:
            f.write(content)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Service creation tool.")
    parser.add_argument("-n", "--cpp_name", required=True, help="Cpp name for the service.")
    parser.add_argument("-c", "--cmake_target", required=True, help="CMake target name.")
    parser.add_argument("-p", "--path", required=True, help="Base path for the service.")
    parser.add_argument("-s", "--service_name", required=True, help="Service name.")

    args = parser.parse_args()

    create_service_structure(args.path, args.service_name, args.cmake_target, args.cpp_name)
