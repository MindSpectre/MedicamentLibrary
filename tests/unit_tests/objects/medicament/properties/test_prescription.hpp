#pragma once

#include <gtest/gtest.h>

#include "medicament_properties/prescription.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::medicaments;
TEST(PrescriptionTest, DefaultConstructor)
{
    const Prescription prescription_drug;
    EXPECT_EQ(prescription_drug.get_description(), "");
}

TEST(PrescriptionTest, ParameterizedConstructorWithJson)
{
    Json::Value properties;
    properties[Prescription::names_of_json_fields::description] =
        "Take twice daily";

    const Prescription prescription_drug(properties);
    EXPECT_EQ(prescription_drug.get_description(), "Take twice daily");
}

TEST(PrescriptionTest, ParameterizedConstructorWithString)
{
    const Prescription prescription_drug("For high blood pressure");

    EXPECT_EQ(prescription_drug.get_description(), "For high blood pressure");
}

TEST(PrescriptionTest, GetInfo)
{
    const Prescription prescription_drug("Use only as directed");

    Json::Value info = prescription_drug.get_info();
    ASSERT_TRUE(info.isObject());
    EXPECT_EQ(
        info[Prescription::names_of_json_fields::description].asString(),
        "Use only as directed");
}

TEST(PrescriptionTest, SetInfo)
{
    Json::Value properties;
    properties[Prescription::names_of_json_fields::description] =
        "Take with food";

    Prescription prescription_drug;
    prescription_drug.set_info(properties);

    EXPECT_EQ(prescription_drug.get_description(), "Take with food");
}

TEST(PrescriptionTest, SetDescription)
{
    Prescription prescription_drug;
    prescription_drug.set_description("For pain relief");

    EXPECT_EQ(prescription_drug.get_description(), "For pain relief");
}

TEST(PrescriptionTest, GetName)
{
    const Prescription prescription_drug;
    EXPECT_EQ(prescription_drug.get_name(), properties::prescription);
}
