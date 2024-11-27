#pragma once
#include <gtest/gtest.h>

#include "disease_properties/symptoms.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
using namespace drug_lib::data::objects::diseases;
TEST(SymptomsTest, DefaultConstructor)
{
    const Symptoms symptoms;
    EXPECT_EQ(symptoms.get_data().size(), 0);
}

TEST(SymptomsTest, ParameterizedConstructor)
{
    const std::vector<Symptom> symptoms_list = {
        Symptom("Cough", 3, "2 weeks", "Respiratory", "Persistent cough"),
        Symptom("Fever", 5, "1 week", "General", "High body temperature")
    };
    const Symptoms symptoms(symptoms_list);
    EXPECT_EQ(symptoms.get_data().size(), 2);
    EXPECT_EQ(symptoms.get_data()[0].get_name(), "Cough");
    EXPECT_EQ(symptoms.get_data()[1].get_name(), "Fever");
}

TEST(SymptomsTest, SetSymptoms)
{
    Symptoms symptoms;
    const std::vector symptoms_list = {
        Symptom("Headache", 4, "3 days", "Neurological", "Throbbing pain")
    };
    symptoms.set_data(symptoms_list);
    EXPECT_EQ(symptoms.get_data().size(), 1);
    EXPECT_EQ(symptoms.get_data()[0].get_name(), "Headache");
}

TEST(SymptomsTest, EmplaceBack)
{
    Symptoms symptoms;
    symptoms.emplace_back("Nausea", 2, "1 day", "Digestive", "Feeling of sickness");
    EXPECT_EQ(symptoms.get_data().size(), 1);
    EXPECT_EQ(symptoms.get_data()[0].get_name(), "Nausea");
}

TEST(SymptomsTest, GetInfo)
{
    const std::vector symptoms_list = {
        Symptom("Cough", 3, "2 weeks", "Respiratory", "Persistent cough"),
        Symptom("Fever", 5, "1 week", "General", "High body temperature")
    };
    const Symptoms symptoms(symptoms_list);

    Json::Value info = symptoms.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0]["name"].asString(), "Cough");
    EXPECT_EQ(info[1]["name"].asString(), "Fever");
}

TEST(SymptomsTest, SetInfo)
{
    Json::Value symptoms_json(Json::arrayValue);
    Json::Value symptom1;
    symptom1["name"] = "Dizziness";
    symptom1["severity"] = 2;
    symptom1["duration"] = "3 days";
    symptom1["type"] = "Neurological";
    symptom1["description"] = "Feeling lightheaded";
    symptoms_json.append(symptom1);

    Symptoms symptoms;
    symptoms.set_info(symptoms_json);

    EXPECT_EQ(symptoms.get_data().size(), 1);
    EXPECT_EQ(symptoms.get_data()[0].get_name(), "Dizziness");
    EXPECT_EQ(symptoms.get_data()[0].get_description(), "Feeling lightheaded");
}

TEST(SymptomsTest, FactoryCreate)
{
    const Symptoms symptoms;
    EXPECT_NO_THROW(data::PropertyFactory::create<Symptoms>(symptoms));
    EXPECT_NO_THROW(data::PropertyFactory::create(diseases::properties::symptoms, Json::arrayValue));
}
