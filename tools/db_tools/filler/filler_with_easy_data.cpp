#include <chrono>
#include <db_interface_factory.hpp>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "stopwatch.hpp"
#include "super_handbook.hpp"

constexpr int32_t record_limit = 1e5;
constexpr int32_t batch_size = 1e3;
std::mutex pool_mutex;
using namespace drug_lib::data::objects;

template <typename T>
void write_to_csv(auto path)
{
    // Implementation to write T objects to CSV (if needed later).
}

void create_meds(drug_lib::common::database::creational::DbInterfacePool &db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking meds");
    drug_lib::dao::SuperHandbook super_handbook; {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    for (int32_t batch_n = 0; batch_n < record_limit / batch_size; ++batch_n)
    {
        std::vector<Medicament> medicaments;
        for (int32_t i = 0; i < batch_size; ++i)
        {
            const int32_t index = batch_n * batch_size + i;
            std::string name = "medicament" + std::to_string(index);
            const bool req_pre = i % 2;
            std::string type = "medicament" + std::to_string(req_pre);
            std::string approval_num = "AVB" + std::to_string(index) + "SYS" + type;
            std::string approval_status = i % 4 ? "accepted" : "rejected";
            std::string atc_code = "ATC" + std::to_string((index + 11) % 3) + "DE" + type;
            Medicament medicament(drug_lib::common::database::Uuid(), std::move(name), std::move(type), req_pre,
                                  std::move(approval_num), std::move(approval_status),
                                  std::move(atc_code));
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<medicaments::Prescription>(
                    i & 1 ? "Required" : "Likely"));
            std::vector active_substances = {
                ::medicaments::ActiveIngredient("Paracetamol", 2),
                ::medicaments::ActiveIngredient("Caffeine", 3)
            };
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<::medicaments::ActiveIngredients>(std::move(active_substances))
            );

            // Add Dosage Form
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<::medicaments::DosageForm>("Tablet, film-coated")
            );

            // Add Inactive Ingredients
            std::vector inactive_substances = {
                ::medicaments::InactiveIngredient("Microcrystalline Cellulose", 1),
                ::medicaments::InactiveIngredient("Magnesium Stearate", 1)
            };
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<::medicaments::InactiveIngredients>(
                    std::move(inactive_substances))
            );

            // Add Side Effects
            std::vector<std::string> side_effects = {
                "Nausea",
                "Dizziness",
                "Fatigue"
            };
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<::medicaments::SideEffects>(std::move(side_effects))
            );

            // Add Strength
            medicament.add_property(
                drug_lib::data::PropertyFactory::create<::medicaments::Strength>("500 mg per tablet")
            );
            if (i == 0 and batch_n == 0)
            {
                std::cout << medicament.to_json() << std::endl;
            }
            medicaments.push_back(std::move(medicament));
        }
        // std::cout << "Insert medicament package: " << batch_n + 1 << std::endl;
        super_handbook.medicaments().force_insert(medicaments);
    }
    stopwatch.finish();
}

void create_diseases(drug_lib::common::database::creational::DbInterfacePool &db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking diseases");
    drug_lib::dao::SuperHandbook super_handbook; {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    for (int32_t batch_n = 0; batch_n < record_limit / batch_size; ++batch_n)
    {
        std::vector<Disease> diseases;
        for (int32_t i = 0; i < batch_size; ++i)
        {
            const int32_t index = batch_n * batch_size + i;
            std::string name = "disease" + std::to_string(index);
            const bool is_infectious = (i % 2 == 0);
            std::string type = is_infectious ? "infectious" : "non-infectious";
            const std::vector symptoms_list = {
                diseases::Symptom("Cough", "r", "2 weeks", "Respiratory",
                                  "Persistent cough"),
                diseases::Symptom("Fever", "r", "1 week", "General",
                                  "High body temperature")
            };
            diseases::Symptoms symptoms(symptoms_list);
            Disease disease(drug_lib::common::database::Uuid(), std::move(name), std::move(type), is_infectious);
            disease.add_property(
                drug_lib::data::PropertyFactory::create<diseases::Symptoms>(
                    std::move(symptoms)));
            disease.add_property(
                drug_lib::data::PropertyFactory::create<diseases::CurativeDrugs>(std::vector{1, 2, 3}));
            // Adding AffectedAgeGroups property
            disease.add_property(
                drug_lib::data::PropertyFactory::create<diseases::AffectedAgeGroups>(
                    std::vector<std::string>{"Children", "Adults", "Elderly"}));

            // Adding Complications property
            disease.add_property(
                drug_lib::data::PropertyFactory::create<diseases::Complications>(
                    std::vector<std::string>{"Pneumonia", "Organ failure", "Sepsis"}));

            // Adding RiskFactors property
            disease.add_property(
                drug_lib::data::PropertyFactory::create<diseases::RiskFactors>(
                    std::vector<std::string>{"Smoking", "Obesity", "High blood pressure"}));
            if (i == 0 and batch_n == 1)
            {
                std::cout << disease.to_json() << std::endl;
            }
            diseases.push_back(std::move(disease));
        }
        // std::cout << "Insert disease package: " << batch_n + 1 << std::endl;
        super_handbook.diseases().force_insert(diseases);
    }
    stopwatch.finish();
}

void create_patients(drug_lib::common::database::creational::DbInterfacePool &db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking patients");
    drug_lib::dao::SuperHandbook super_handbook; {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution year_dist(1960, 2022);
    std::uniform_int_distribution<int> month_dist(1, 12);
    std::uniform_int_distribution<int> day_dist(1, 28);

    for (int32_t batch_n = 0; batch_n < record_limit / batch_size; ++batch_n)
    {
        std::vector<Patient> patients;
        for (int32_t i = 0; i < batch_size; ++i)
        {
            const int32_t index = batch_n * batch_size + i;
            std::string name = "patient" + std::to_string(index);
            std::string gender = (i % 2 == 0) ? "Male" : "Female";

            int year = year_dist(generator);
            int month = month_dist(generator);
            int day = day_dist(generator);

            std::chrono::year_month_day birth_date{
                std::chrono::year(year), std::chrono::month(month),
                std::chrono::day(day)
            };
            std::string contact_info = "patient" + std::to_string(index) + "@example.com";

            Patient patient(drug_lib::common::database::Uuid(), std::move(name), std::move(gender), birth_date,
                            std::move(contact_info));
            std::vector<drug_lib::common::database::Uuid> diseases = {drug_lib::common::database::Uuid("101")};
            patients::CurrentDiseases current_diseases(std::move(diseases));
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::CurrentDiseases>(
                    std::move(current_diseases)));
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::CurrentMedicaments>(std::vector<drug_lib::common::database::Uuid>{drug_lib::common::database::Uuid("1")}));
            // Adding Allergies property
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::Allergies>(
                    std::vector<std::string>{"Peanuts", "Dust", "Pollen"}));

            // Adding BloodType property
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::BloodType>("O+"));
            // Replace "O+" with the actual blood type as required

            // Adding Insurance property
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::Insurance>("PremiumCare"));
            // Replace "PremiumCare" with the actual insurance name as required

            // Adding MedicalHistory property
            std::vector<patients::HealthRecord> medical_history = {
                {drug_lib::common::database::Uuid("101"), std::chrono::year{2022} / std::chrono::month{5}, std::chrono::year{2023} / std::chrono::month{2}},
                {drug_lib::common::database::Uuid("102"), std::chrono::year{2023} / std::chrono::month{3}} // Ongoing disease without an end date
            };
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::MedicalHistory>(std::move(medical_history)));

            // Adding Vaccines property
            patient.add_property(
                drug_lib::data::PropertyFactory::create<patients::Vaccines>(
                    std::vector<std::string>{"COVID-19", "Influenza", "Rabies"}));
            if (i == 0 and batch_n == 2)
            {
                std::cout << patient.to_json() << std::endl;
            }
            patients.push_back(std::move(patient));
        }
        // std::cout << "Insert patient package: " << batch_n + 1 << std::endl;
        super_handbook.patients().force_insert(patients);
    }
    stopwatch.finish();
}

void create_organizations(drug_lib::common::database::creational::DbInterfacePool &db_pool)
{
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start("Mocking organizations");
    drug_lib::dao::SuperHandbook super_handbook; {
        std::lock_guard lock(pool_mutex);
        super_handbook.establish_from_pool(db_pool);
    }
    for (int32_t batch_n = 0; batch_n < record_limit / batch_size; ++batch_n)
    {
        std::vector<Organization> organizations;
        for (int32_t i = 0; i < batch_size; ++i)
        {
            const int32_t index = batch_n * batch_size + i;
            std::string name = "organization" + std::to_string(index);
            std::string type = (i % 2 == 0) ? "Hospital" : "Clinic";
            std::string country = "Country" + std::to_string(i % 100);
            std::string contact_details = "org" + std::to_string(index) + "@example.com";

            Organization organization(drug_lib::common::database::Uuid(), std::move(name), std::move(type),
                                      std::move(country), std::move(contact_details));
            organizations::License license;
            license.set_license_name("Sample License");
            license.set_license_key("SAMPLE123");
            organization.add_property(
                drug_lib::data::PropertyFactory::create<organizations::License>(
                    std::move(license)));
            if (i == 0 and batch_n == 4)
            {
                std::cout << organization.to_json() << std::endl;
            }
            organizations.push_back(std::move(organization));
        }
        // std::cout << "Insert organization package: " << batch_n + 1 << std::endl;
        super_handbook.organizations().force_insert(organizations);
    }
    stopwatch.finish();
}

int main()
{
    constexpr uint32_t port = 5432;
    constexpr auto host = "localhost";
    constexpr auto db_name = "test_db";
    constexpr auto username = "postgres";
    constexpr auto password = "postgres";
    drug_lib::common::database::creational::DbInterfacePool db_pool;
    drug_lib::common::database::PqxxConnectParams connect_params{host, port, db_name, username, password};
    db_pool.fill(4, drug_lib::common::database::creational::DbInterfaceFactory::create_pqxx_client, connect_params);

    // Run threads
    std::jthread meds_thread(create_meds, std::ref(db_pool));
    std::jthread diseases_thread(create_diseases, std::ref(db_pool));
    std::jthread patients_thread(create_patients, std::ref(db_pool));
    std::jthread organizations_thread(create_organizations, std::ref(db_pool));

    return 0;
}
