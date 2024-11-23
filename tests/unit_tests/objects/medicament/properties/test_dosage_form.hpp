#pragma once

#include <gtest/gtest.h>

#include "medicament_properties/prescription.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::medicaments;
TEST(DosageFormTest, DefaultConstructor)
{
    const DosageForm dosage_form;
    EXPECT_EQ(dosage_form.get_description(), "");
}

TEST(DosageFormTest, ParameterizedConstructorWithJson)
{
    Json::Value properties;
    properties[DosageForm::names_of_json_fields::description] =
        "Take twice daily";

    const DosageForm dosage_form(properties);
    EXPECT_EQ(dosage_form.get_description(), "Take twice daily");
}

TEST(DosageFormTest, ParameterizedConstructorWithString)
{
    const DosageForm dosage_form("For high blood pressure");

    EXPECT_EQ(dosage_form.get_description(), "For high blood pressure");
}

TEST(DosageFormTest, GetInfo)
{
    const DosageForm dosage_form("Use only as directed");

    Json::Value info = dosage_form.get_info();
    ASSERT_TRUE(info.isObject());
    EXPECT_EQ(
        info[DosageForm::names_of_json_fields::description].asString(),
        "Use only as directed");
}

TEST(DosageFormTest, SetInfo)
{
    Json::Value properties;
    properties[DosageForm::names_of_json_fields::description] =
        "Take with food";

    DosageForm dosage_form;
    dosage_form.set_info(properties);

    EXPECT_EQ(dosage_form.get_description(), "Take with food");
}

TEST(DosageFormTest, SetDescription)
{
    DosageForm dosage_form;
    dosage_form.set_description("For pain relief");

    EXPECT_EQ(dosage_form.get_description(), "For pain relief");
}

TEST(DosageFormTest, GetName)
{
    const DosageForm dosage_form;
    EXPECT_EQ(dosage_form.get_name(), properties::dosage_form);
}
