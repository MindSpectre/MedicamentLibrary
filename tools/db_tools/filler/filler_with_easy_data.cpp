#include <db_interface_factory.hpp>
#include "stopwatch.hpp"
#include "super_handbook.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <mutex>
#include <random>
#include <thread>

constexpr int32_t RECORD_LIMIT = 1e6;
constexpr int32_t BATCH_SIZE = 1e4;
std::mutex pool_mutex;

template <typename T>
void write_to_csv(std::string_view path)
{
    // Implementation to write T objects to CSV (if needed later).
}

void create_meds(drug_lib::common::database::creational::DbInterfacePool& db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking meds");
    drug_lib::dao::SuperHandbook super_handbook;
    {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    for (int32_t batch_n = 0; batch_n < RECORD_LIMIT / BATCH_SIZE; ++batch_n)
    {
        std::vector<drug_lib::data::objects::Medicament> medicaments;
        for (int32_t i = 0; i < BATCH_SIZE; ++i)
        {
            const int32_t index = batch_n * BATCH_SIZE + i;
            std::string name = "medicament" + std::to_string(index);
            const bool req_pre = i % 2;
            std::string type = "medicament" + std::to_string(req_pre);
            std::string approval_num = "AVB" + std::to_string(index) + "SYS" + type;
            std::string approval_status = i % 4 ? "accepted" : "rejected";
            std::string atc_code = "ATC" + std::to_string((index + 11) % 3) + "DE" + type;
            drug_lib::data::objects::Medicament medicament(index, std::move(name), std::move(type), req_pre, i,
                                                           std::move(approval_num), std::move(approval_status),
                                                           std::move(atc_code));
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<drug_lib::data::objects::medicaments::Prescription>(
                    i & 1 ? "Required" : "Likely"));
            medicaments.push_back(std::move(medicament));
        }
        std::cout << "Insert package: " << batch_n + 1 << std::endl;
        super_handbook.medicaments().force_insert(medicaments);
    }
    stopwatch.finish();
}

void create_diseases(drug_lib::common::database::creational::DbInterfacePool& db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking diseases");
    drug_lib::dao::SuperHandbook super_handbook;
    {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    for (int32_t batch_n = 0; batch_n < RECORD_LIMIT / BATCH_SIZE; ++batch_n)
    {
        std::vector<drug_lib::data::objects::Disease> diseases;
        for (int32_t i = 0; i < BATCH_SIZE; ++i)
        {
            const int32_t index = batch_n * BATCH_SIZE + i;
            std::string name = "disease" + std::to_string(index);
            const bool is_infectious = (i % 2 == 0);
            std::string type = is_infectious ? "infectious" : "non-infectious";
            const std::vector symptoms_list = {
                drug_lib::data::objects::diseases::Symptom("Cough", 3, "2 weeks", "Respiratory",
                                                           "Persistent cough"),
                drug_lib::data::objects::diseases::Symptom("Fever", 5, "1 week", "General",
                                                           "High body temperature")
            };
            drug_lib::data::objects::diseases::Symptoms symptoms(symptoms_list);
            drug_lib::data::objects::Disease disease(index, std::move(name), std::move(type), is_infectious);
            disease.add_property(
                drug_lib::data::PropertyFactory::create<drug_lib::data::objects::diseases::Symptoms>(
                    std::move(symptoms)));
            diseases.push_back(std::move(disease));
        }
        std::cout << "Insert package: " << batch_n + 1 << std::endl;
        super_handbook.diseases().force_insert(diseases);
    }
    stopwatch.finish();
}

void create_patients(drug_lib::common::database::creational::DbInterfacePool& db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking patients");
    drug_lib::dao::SuperHandbook super_handbook;
    {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> year_dist(1960, 2022);
    std::uniform_int_distribution<int> month_dist(1, 12);
    std::uniform_int_distribution<int> day_dist(1, 28);

    for (int32_t batch_n = 0; batch_n < RECORD_LIMIT / BATCH_SIZE; ++batch_n)
    {
        std::vector<drug_lib::data::objects::Patient> patients;
        for (int32_t i = 0; i < BATCH_SIZE; ++i)
        {
            const int32_t index = batch_n * BATCH_SIZE + i;
            std::string name = "patient" + std::to_string(index);
            std::string gender = (i % 2 == 0) ? "Male" : "Female";

            int year = year_dist(generator);
            int month = month_dist(generator);
            int day = day_dist(generator);

            std::chrono::year_month_day birth_date{std::chrono::year(year), std::chrono::month(month),
                                                   std::chrono::day(day)};
            std::string contact_info = "patient" + std::to_string(index) + "@example.com";

            drug_lib::data::objects::Patient patient(index, std::move(name), std::move(gender), birth_date,
                                                     std::move(contact_info));
            std::vector diseases = {101, 202, 303};
            drug_lib::data::objects::patients::CurrentDiseases current_diseases(diseases);
            patient.add_property(
                drug_lib::data::PropertyFactory::create<drug_lib::data::objects::patients::CurrentDiseases>(
                    std::move(current_diseases)));
            patients.push_back(std::move(patient));
        }
        std::cout << "Insert package: " << batch_n + 1 << std::endl;
        super_handbook.patients().force_insert(patients);
    }
    stopwatch.finish();
}

void create_orgs(drug_lib::common::database::creational::DbInterfacePool& db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking organizations");
    drug_lib::dao::SuperHandbook super_handbook;
    {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    for (int32_t batch_n = 0; batch_n < RECORD_LIMIT / BATCH_SIZE; ++batch_n)
    {
        std::vector<drug_lib::data::objects::Organization> organizations;
        for (int32_t i = 0; i < BATCH_SIZE; ++i)
        {
            const int32_t index = batch_n * BATCH_SIZE + i;
            std::string name = "organization" + std::to_string(index);
            std::string type = (i % 2 == 0) ? "Hospital" : "Clinic";
            std::string country = "Country" + std::to_string(i % 100);
            std::string contact_details = "org" + std::to_string(index) + "@example.com";

            drug_lib::data::objects::Organization organization(index, std::move(name), std::move(type),
                                                               std::move(country), std::move(contact_details));
            drug_lib::data::objects::organizations::License license;
            license.set_license_name("Sample License");
            license.set_license_key("SAMPLE123");
            organization.add_property(
                drug_lib::data::PropertyFactory::create<drug_lib::data::objects::organizations::License>(
                    std::move(license)));
            organizations.push_back(std::move(organization));
        }
        std::cout << "Insert package: " << batch_n + 1 << std::endl;
        super_handbook.organizations().force_insert(organizations);
    }
    stopwatch.finish();
}

int main()
{
    constexpr uint32_t port = 5432;
    constexpr std::string_view host = "localhost";
    constexpr std::string_view db_name = "test_db";
    constexpr std::string_view username = "postgres";
    constexpr std::string_view password = "postgres";
    drug_lib::common::database::creational::DbInterfacePool db_pool;
    drug_lib::common::database::PqxxConnectParams connect_params{host, port, db_name, username, password};
    db_pool.fill(4, drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client, connect_params);
    // Paths for CSV files
    std::string meds_csv = "meds.csv";
    std::string diseases_csv = "diseases.csv";
    std::string patients_csv = "patients.csv";
    std::string orgs_csv = "orgs.csv";

    // Run threads
    std::jthread meds_thread(create_meds, std::ref(db_pool));
    std::jthread diseases_thread(create_diseases, std::ref(db_pool));
    std::jthread patients_thread(create_patients, std::ref(db_pool));
    std::jthread orgs_thread(create_orgs, std::ref(db_pool));

    return 0;
}
