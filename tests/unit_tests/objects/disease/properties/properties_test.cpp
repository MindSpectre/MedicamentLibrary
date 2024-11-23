#include <gtest/gtest.h>

#include "disease.hpp"
#include "properties_controller.hpp"
#include "test_affected_age_groups.hpp"
#include "test_curative_drugs.hpp"
#include "test_risk_factors.hpp"
#include "test_complications.hpp"
#include "test_symptoms.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects;
TEST(DiseasePropertiesTest, AddProperty)
{
    Disease disease;
    const auto symptoms = data::PropertyFactory::create<diseases::Symptoms>(Json::Value());
    disease.add_property(symptoms);

    const auto retrieved_property = disease.get_property(diseases::properties::symptoms);
    ASSERT_NE(retrieved_property, nullptr);
    EXPECT_EQ(retrieved_property->get_name(), diseases::properties::symptoms);
}

TEST(DiseasePropertiesTest, RemoveProperty)
{
    Disease disease;
    const auto symptoms = data::PropertyFactory::create<diseases::Symptoms>(Json::Value());
    disease.add_property(symptoms);

    disease.remove_property(diseases::properties::symptoms);
    EXPECT_THROW(auto t = disease.get_property(diseases::properties::symptoms), boost::container::out_of_range);
}

TEST(DiseasePropertiesTest, GetPropertyInfo)
{
    Disease disease;
    std::vector symptoms_list = {
        diseases::Symptom("Cough", 3, "2 weeks", "Respiratory", "Persistent cough")
    };
    const auto symptoms = data::PropertyFactory::create<diseases::Symptoms>(std::move(symptoms_list));
    disease.add_property(symptoms);
    auto sym_list = disease.get_property(diseases::properties::symptoms);
    Json::Value info = symptoms->get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info[0]["name"].asString(), "Cough");
}

TEST(DiseasePropertiesTest, ValidationJson)
{
    Disease disease(3, "Chickenpox", "Viral", true);
    std::vector symptoms_list = {
        diseases::Symptom("Cough", 3, "2 weeks", "Respiratory", "Persistent cough")
    };
    const auto symptoms = data::PropertyFactory::create<diseases::Symptoms>(std::move(symptoms_list));
    disease.add_property(symptoms);
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
            EXPECT_EQ(3, field->as<int>());
        }
        else if (field->get_name() == Disease::field_name::properties)
        {
            const auto info = field->as<Json::Value>();
            ASSERT_TRUE(info.isObject());
            EXPECT_TRUE(info.isMember(diseases::properties::symptoms));
            EXPECT_EQ(
                info[diseases::properties::symptoms][0]
                [diseases::Symptom::names_of_json_fields::name].asString(), "Cough");
            EXPECT_EQ(
                info[diseases::properties::symptoms][0]
                [diseases::Symptom::names_of_json_fields::type].asString(), "Respiratory");
            EXPECT_EQ(
                info[diseases::properties::symptoms][0]
                [diseases::Symptom::names_of_json_fields::severity].asInt(), 3);
        }
    }
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
