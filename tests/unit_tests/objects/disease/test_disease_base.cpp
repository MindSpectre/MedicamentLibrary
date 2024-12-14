
#include <gtest/gtest.h>
#include "disease.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;

TEST(DiseaseTest, DefaultConstructor)
{
    const Disease disease;
    EXPECT_EQ(disease.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(disease.get_name(), "");
    EXPECT_EQ(disease.get_type(), "");
    EXPECT_FALSE(disease.is_infectious());
}

TEST(DiseaseTest, ParameterizedConstructor)
{
    const Disease disease(common::database::Uuid(), "Flu", "Viral", true);
    EXPECT_EQ(disease.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(disease.get_name(), "Flu");
    EXPECT_EQ(disease.get_type(), "Viral");
    EXPECT_TRUE(disease.is_infectious());
}

TEST(DiseaseTest, SettersAndGetters)
{
    Disease disease;
    disease.set_uuid(common::database::Uuid());
    disease.set_name("Cold");
    disease.set_type("Viral");
    disease.set_infectious(false);

    EXPECT_EQ(disease.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(disease.get_name(), "Cold");
    EXPECT_EQ(disease.get_type(), "Viral");
    EXPECT_FALSE(disease.is_infectious());
}

TEST(DiseaseTest, ToRecord)
{
    const Disease disease(common::database::Uuid(), "Chickenpox", "Viral", true);
    for (const auto record = disease.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == Disease::field_name::name)
        {
            EXPECT_EQ("Chickenpox", field->as<std::string>());
        }
        else if (field->get_name() == Disease::field_name::type)
        {
            EXPECT_EQ("Viral", field->as<std::string>());
        }
        else if (field->get_name() == Disease::field_name::is_infectious)
        {
            EXPECT_TRUE(field->as<bool>());
        }
        else if (field->get_name() == Disease::field_name::id)
        {
            EXPECT_EQ(common::database::Uuid(), field->as<common::database::Uuid>());
        }
    }
}

TEST(DiseaseTest, FromRecord)
{
    common::database::Record record;
    record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(Disease::field_name::id, common::database::Uuid()));
    record.push_back(std::make_unique<common::database::Field<std::string>>(Disease::field_name::name, "Measles"));
    record.push_back(std::make_unique<common::database::Field<std::string>>(Disease::field_name::type, "Viral"));
    record.push_back(std::make_unique<common::database::Field<bool>>(Disease::field_name::is_infectious, true));

    Disease disease;
    disease.from_record(record);

    EXPECT_EQ(disease.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(disease.get_name(), "Measles");
    EXPECT_EQ(disease.get_type(), "Viral");
    EXPECT_TRUE(disease.is_infectious());
}

Json::Value createDiseaseJson()
{
    Json::Value disease;

    disease["id"] = common::database::Uuid().get_id();
    disease["is_infectious"] = true;
    disease["name"] = "disease3";
    disease["type"] = "infectious";

    // Property object
    Json::Value properties;

    // Affected age groups
    properties["affected_age_groups"].append("Children");
    properties["affected_age_groups"].append("Adults");
    properties["affected_age_groups"].append("Elderly");

    // Complications
    properties["complications"].append("Pneumonia");
    properties["complications"].append("Organ failure");
    properties["complications"].append("Sepsis");

    // Curative drugs
    properties["curative_drugs"].append(1);
    properties["curative_drugs"].append(2);
    properties["curative_drugs"].append(3);

    // Risk factors
    properties["risk_factors"].append("Smoking");
    properties["risk_factors"].append("Obesity");
    properties["risk_factors"].append("High blood pressure");

    // Symptom array
    Json::Value symptom1;
    symptom1["description"] = "Persistent cough";
    symptom1["duration"] = "2 weeks";
    symptom1["name"] = "Cough";
    symptom1["severity"] = "low";
    symptom1["type"] = "Respiratory";

    Json::Value symptom2;
    symptom2["description"] = "High body temperature";
    symptom2["duration"] = "1 week";
    symptom2["name"] = "Fever";
    symptom2["severity"] = "med";
    symptom2["type"] = "General";

    properties["symptoms"].append(symptom1);
    properties["symptoms"].append(symptom2);

    // Add properties to disease
    disease["properties"] = properties;

    return disease;
}

Disease createDisease()
{
    constexpr int32_t i = 2;
    constexpr int32_t index = 1 + i;
    std::string name = "disease" + std::to_string(index);
    constexpr bool is_infectious = (i % 2 == 0);
    std::string type = "infectious";
    const std::vector symptoms_list = {
        diseases::Symptom("Cough", "low", "2 weeks", "Respiratory",
                          "Persistent cough"),
        diseases::Symptom("Fever", "med", "1 week", "General",
                          "High body temperature")
    };
    diseases::Symptoms symptoms(symptoms_list);
    Disease disease(common::database::Uuid(), std::move(name), std::move(type), is_infectious);
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

    return disease;
}

TEST(DiseaseTest, ToJson)
{

    Disease em = createDisease();
    EXPECT_EQ(em.to_json(), createDiseaseJson());
}

TEST(DiseaseTest, FromJson)
{

    Disease em;
    EXPECT_NO_THROW(em.from_json(createDiseaseJson()));
    EXPECT_EQ(em, createDisease());
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
