#pragma once

#include <gtest/gtest.h>
#include "patient_properties/vaccines.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(VaccinesTest, DefaultConstructor)
{
    const Vaccines vaccines;
    EXPECT_EQ(vaccines.get_info().size(), 0);
}

TEST(VaccinesTest, ParameterizedConstructorWithVector)
{
    const std::vector<std::string> vaccine_list = {"COVID-19", "Influenza", "Hepatitis B"};
    const Vaccines vaccines(vaccine_list);

    Json::Value info = vaccines.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asString(), "COVID-19");
    EXPECT_EQ(info[1].asString(), "Influenza");
    EXPECT_EQ(info[2].asString(), "Hepatitis B");
}

TEST(VaccinesTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("Polio");
    properties.append("Measles");
    properties.append("Chickenpox");

    const Vaccines vaccines(properties);

    Json::Value info = vaccines.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asString(), "Polio");
    EXPECT_EQ(info[1].asString(), "Measles");
    EXPECT_EQ(info[2].asString(), "Chickenpox");
}

TEST(VaccinesTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("Rabies");
    properties.append("Tetanus");

    Vaccines vaccines;
    vaccines.set_info(properties);

    Json::Value info = vaccines.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asString(), "Rabies");
    EXPECT_EQ(info[1].asString(), "Tetanus");
}

TEST(VaccinesTest, SetInfoInvalidJson)
{
    const Json::Value properties(Json::objectValue); // Not an array, should trigger exception

    Vaccines vaccines;
    EXPECT_THROW(vaccines.set_info(properties), std::invalid_argument);
}

TEST(VaccinesTest, GetName)
{
    const Vaccines vaccines;
    EXPECT_EQ(vaccines.get_name(), properties::vaccines);
}
