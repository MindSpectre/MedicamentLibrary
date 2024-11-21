#pragma once

#include <gtest/gtest.h>

#include "medicament_properties/prescription.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::medicaments;
TEST(PrescriptionDrugTest, DefaultConstructor)
{
    const PrescriptionDrug prescription_drug;
    EXPECT_EQ(prescription_drug.get_description(), "");
}

TEST(PrescriptionDrugTest, ParameterizedConstructorWithJson)
{
    Json::Value properties;
    properties[PrescriptionDrug::names_of_json_fields::description] =
        "Take twice daily";

    const PrescriptionDrug prescription_drug(properties);
    EXPECT_EQ(prescription_drug.get_description(), "Take twice daily");
}

TEST(PrescriptionDrugTest, ParameterizedConstructorWithString)
{
    const PrescriptionDrug prescription_drug("For high blood pressure");

    EXPECT_EQ(prescription_drug.get_description(), "For high blood pressure");
}

TEST(PrescriptionDrugTest, GetInfo)
{
    const PrescriptionDrug prescription_drug("Use only as directed");

    Json::Value info = prescription_drug.get_info();
    ASSERT_TRUE(info.isObject());
    EXPECT_EQ(
        info[PrescriptionDrug::names_of_json_fields::description].asString(),
        "Use only as directed");
}

TEST(PrescriptionDrugTest, SetInfo)
{
    Json::Value properties;
    properties[PrescriptionDrug::names_of_json_fields::description] =
        "Take with food";

    PrescriptionDrug prescription_drug;
    prescription_drug.set_info(properties);

    EXPECT_EQ(prescription_drug.get_description(), "Take with food");
}

TEST(PrescriptionDrugTest, SetDescription)
{
    PrescriptionDrug prescription_drug;
    prescription_drug.set_description("For pain relief");

    EXPECT_EQ(prescription_drug.get_description(), "For pain relief");
}

TEST(PrescriptionDrugTest, GetName)
{
    const PrescriptionDrug prescription_drug;
    EXPECT_EQ(prescription_drug.get_name(), properties::prescription);
}
