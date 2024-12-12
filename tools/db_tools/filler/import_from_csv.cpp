#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <json/json.h> // For creating JSON objects
#include "csv.h" // A CSV parser library like csv-parser or similar
#include "db_interface_factory.hpp"
#include "db_interface_pool.hpp"
#include "medicament.hpp"
#include "pqxx_connect_params.hpp"

std::vector<Json::Value> csv_to_json(const std::string& csv_file) {
    std::vector<Json::Value> json_records;

    rapidcsv::Document csv_doc(csv_file);
    for (std::size_t i = 0; i < csv_doc.GetRowCount(); ++i)
    {
        std::vector<std::string> row = csv_doc.GetRow<std::string>(i);
        Json::Value record;
        for (int j = 0; j < csv_doc.GetColumnCount(); ++j)
        {
            if (csv_doc.GetColumnName(j) == "properties")
            {
                boost::algorithm::replace_all(row[5], "\'", "\"");
                Json::CharReaderBuilder readerBuilder;
                std::string errs;
                std::istringstream propertiesStream(row[5]);

                Json::Value propertiesJson;
                if (!parseFromStream(readerBuilder, propertiesStream, &propertiesJson, &errs)) {
                    std::cerr << "Failed to parse properties JSON: " << errs << std::endl;
                    continue;
                }
                record["properties"] = propertiesJson;
                continue;
            }
            record[csv_doc.GetColumnName(j)] = row[j];
        }


        // Add the JSON object to the list
        json_records.push_back(record);
    }

    return json_records;
}

// Example usage
int main() {
    const std::string csv_file = "csv/medicaments.csv";

    // Convert CSV to JSON
    std::vector<Json::Value> json_records = csv_to_json(csv_file);

    drug_lib::data::objects::Medicament test_medicament;
    test_medicament.from_json(json_records[0]);
    std::cout << test_medicament.to_json() << std::endl;
    return 0;
    constexpr uint32_t port = 5432;
    constexpr auto host = "localhost";
    constexpr auto db_name = "test_db";
    constexpr auto username = "postgres";
    constexpr auto password = "postgres";
    drug_lib::common::database::creational::DbInterfacePool db_pool;
    drug_lib::common::database::PqxxConnectParams connect_params{host, port, db_name, username, password};
    db_pool.fill(4, drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client, connect_params);
    const std::string drugs_csv_file = "csv/medicaments.csv";
    const std::string patients_csv_file = "csv/medicaments.csv";
    const std::string organizations_csv_file = "csv/medicaments.csv";
    const std::string diseases_csv_file = "csv/medicaments.csv";
    // Convert CSV to JSON
    std::vector<Json::Value> meds = csv_to_json(drugs_csv_file);

    return 0;
}
