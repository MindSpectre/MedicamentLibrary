#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "objects.hpp"
using namespace drug_lib::data::objects;

void create_meds(const std::string &file_name)
{
	constexpr int32_t index = 12;
	std::string name = "medicament" + std::to_string(index);
	constexpr bool req_pre = true;
	std::string type = "medicament" + std::to_string(req_pre);
	std::string approval_num = "AVB" + std::to_string(index) + "SYS" + type;
	std::string approval_status = "accepted";
	std::string atc_code = "ATC" + std::to_string((index + 11) % 3) + "DE" + type;
	Medicament medicament(
		drug_lib::common::database::Uuid(), std::move(name), std::move(type), req_pre,
		std::move(approval_num), std::move(approval_status),
		std::move(atc_code));
	medicament.add_property(
		drug_lib::data::PropertyFactory::create<medicaments::Prescription>(
			"required"));
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
	std::ofstream file( file_name);
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " <<  file_name << std::endl;
		return;
	}
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "  "; // For pretty formatting (2 spaces)
	file << Json::writeString(writer, medicament.to_json());

	// Close the file
	file.close();
}

void create_diseases(const std::string &file_name)
{
	constexpr int32_t index = 13;
	std::string name = "disease" + std::to_string(index);
	constexpr bool is_infectious = true;
	std::string type = "Bacterial";
	const std::vector symptoms_list = {
		diseases::Symptom(
			"Cough", "r", "2 weeks", "Respiratory",
			"Persistent cough"),
		diseases::Symptom(
			"Fever", "r", "1 week", "General",
			"High body temperature")
	};
	diseases::Symptoms symptoms(symptoms_list);
	Disease disease(drug_lib::common::database::Uuid(), std::move(name), std::move(type), is_infectious);
	disease.add_property(
		drug_lib::data::PropertyFactory::create<diseases::Symptoms>(
			std::move(symptoms)));
	disease.add_property(
		drug_lib::data::PropertyFactory::create<diseases::CurativeDrugs>(std::vector{drug_lib::common::database::Uuid(), drug_lib::common::database::Uuid()}));
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

	std::ofstream file( file_name);
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " <<  file_name << std::endl;
		return;
	}
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "  "; // For pretty formatting (2 spaces)
	file << Json::writeString(writer, disease.to_json());

	// Close the file
	file.close();
}

void create_patients(const std::string &file_name)
{
	std::default_random_engine generator(std::random_device{}());
	std::uniform_int_distribution year_dist(1960, 2022);
	std::uniform_int_distribution<int> month_dist(1, 12);
	std::uniform_int_distribution<int> day_dist(1, 28);
	constexpr int32_t index = 13;
	std::string name = "patient" + std::to_string(index);
	std::string gender = "male";

	int year = year_dist(generator);
	int month = month_dist(generator);
	int day = day_dist(generator);

	std::chrono::year_month_day birth_date{
		std::chrono::year(year), std::chrono::month(month),
		std::chrono::day(day)
	};
	std::string contact_info = "patient" + std::to_string(index) + "@example.com";

	Patient patient(
		drug_lib::common::database::Uuid(), std::move(name), std::move(gender), birth_date,
		std::move(contact_info));
	std::vector diseases = {drug_lib::common::database::Uuid("101", false)};
	patients::CurrentDiseases current_diseases(std::move(diseases));
	patient.add_property(
		drug_lib::data::PropertyFactory::create<patients::CurrentDiseases>(
			std::move(current_diseases)));
	patient.add_property(
		drug_lib::data::PropertyFactory::create<patients::CurrentMedicaments>(std::vector{drug_lib::common::database::Uuid("1", false)}));
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
		{drug_lib::common::database::Uuid("101", false), std::chrono::year{2022} / std::chrono::month{5}, std::chrono::year{2023} / std::chrono::month{2}},
		{drug_lib::common::database::Uuid("102", false), std::chrono::year{2023} / std::chrono::month{3}} // Ongoing disease without an end date
	};
	patient.add_property(
		drug_lib::data::PropertyFactory::create<patients::MedicalHistory>(std::move(medical_history)));

	// Adding Vaccines property
	patient.add_property(
		drug_lib::data::PropertyFactory::create<patients::Vaccines>(
			std::vector<std::string>{"COVID-19", "Influenza", "Rabies"}));
	std::ofstream file( file_name);
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " <<  file_name << std::endl;
		return;
	}
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "  "; // For pretty formatting (2 spaces)
	file << Json::writeString(writer, patient.to_json());

	// Close the file
	file.close();
}

void create_organizations(const std::string &file_name)
{
	constexpr int32_t index = 13;
	std::string name = "organization" + std::to_string(index);
	std::string type = "Clinic";
	std::string country = "USA";
	std::string contact_details = "org" + std::to_string(index) + "@example.com";

	Organization organization(
		drug_lib::common::database::Uuid(), std::move(name), std::move(type),
		std::move(country), std::move(contact_details));
	organizations::License license;
	license.set_license_name("Sample License");
	license.set_license_key("SAMPLE123");
	organization.add_property(
		drug_lib::data::PropertyFactory::create<organizations::License>(
			std::move(license)));
	std::ofstream file( file_name);
	if (!file.is_open())
	{
		std::cerr << "Error opening file: " <<  file_name << std::endl;
		return;
	}
	Json::StreamWriterBuilder writer;
	writer["indentation"] = "  "; // For pretty formatting (2 spaces)
	file << Json::writeString(writer, organization.to_json());

	// Close the file
	file.close();
}

int main()
{
	create_patients("patients.json");
	create_organizations("organizations.json");
	create_diseases("diseases.json");
	create_meds("medicaments.json");
	return 0;
}
