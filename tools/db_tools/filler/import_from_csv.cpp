#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <boost/algorithm/string/replace.hpp>
#include <json/json.h> // For creating JSON objects

#include "csv.h" // A CSV parser library like csv-parser or similar
#include "db_interface_factory.hpp"
#include "db_interface_pool.hpp"
#include "disease.hpp"
#include "pqxx_connect_params.hpp"

std::vector<Json::Value> csv_to_json(const std::string& csv_file) {
    std::vector<Json::Value> json_records;

    try {
        // Initialize the CSV document. Adjust LabelParams if headers are different.
        rapidcsv::Document csv_doc(csv_file);

        // Iterate over each row in the CSV
        for (std::size_t i = 0; i < csv_doc.GetRowCount(); ++i) {
            std::vector<std::string> row = csv_doc.GetRow<std::string>(i);
            Json::Value record;

            // Iterate over each column in the row
            for (std::size_t j = 0; j < csv_doc.GetColumnCount(); ++j) {
                std::string column_name = csv_doc.GetColumnName(j);
                std::string cell_value = row[j];

                if (column_name == "properties") {
                    // Replace single quotes with double quotes to form valid JSON
                    boost::algorithm::replace_all(cell_value, "'", "\"");

                    std::string errs;
                    std::istringstream properties_stream(cell_value);
                    Json::Value properties_json;
                    Json::CharReaderBuilder reader_builder;

                    // Parse the JSON from the properties column
                    if (!Json::parseFromStream(reader_builder, properties_stream, &properties_json, &errs)) {
                        std::cerr << "Failed to parse properties JSON in row " << i+1 << ": " << errs << std::endl;
                        // Optionally, handle the error (e.g., skip the row or set properties to null)
                        // Here, we'll skip adding the properties field
                        continue;
                    }

                    record["properties"] = properties_json;
                } else {
                    // For other columns, add them directly to the record
                    record[column_name] = cell_value;
                }
            }

            // Add the JSON object to the list
            json_records.push_back(record);
        }
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }

    return json_records;
}


// Example usage
int main() {
    constexpr uint32_t port = 5432;
    constexpr auto host = "localhost";
    constexpr auto db_name = "test_db";
    constexpr auto username = "postgres";
    constexpr auto password = "postgres";
    drug_lib::common::database::creational::DbInterfacePool db_pool;
    drug_lib::common::database::PqxxConnectParams connect_params{host, port, db_name, username, password};
    db_pool.fill(4, drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client, connect_params);
    const std::string medicaments_csv_file = "csv/medicaments.csv";
    const std::string patients_csv_file = "csv/patients.csv";
    const std::string organizations_csv_file = "csv/organizations.csv";
    const std::string diseases_csv_file = "csv/diseases.csv";
    // Convert CSV to JSON
    std::vector<Json::Value> meds = csv_to_json(medicaments_csv_file);
    std::vector<Json::Value> diseases = csv_to_json(diseases_csv_file);
    std::vector<Json::Value> organizations = csv_to_json(organizations_csv_file);
    std::vector<Json::Value> patients = csv_to_json(patients_csv_file);

    std::cout << patients[0].toStyledString() << std::endl;
    return 0;
}
