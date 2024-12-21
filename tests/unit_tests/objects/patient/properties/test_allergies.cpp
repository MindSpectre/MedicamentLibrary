#pragma once

#include <gtest/gtest.h>
#include "patient_properties/allergies.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(AllergiesTest, DefaultConstructor)
{
    const Allergies allergies;
    EXPECT_EQ(allergies.get_info().size(), 0);
}

TEST(AllergiesTest, ParameterizedConstructorWithVector)
{
    const std::vector<std::string> allergy_list = {"peanuts", "pollen", "shellfish"};
    const Allergies allergies(allergy_list);

    Json::Value info = allergies.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asString(), "peanuts");
    EXPECT_EQ(info[1].asString(), "pollen");
    EXPECT_EQ(info[2].asString(), "shellfish");
}

TEST(AllergiesTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("peanuts");
    properties.append("pollen");
    properties.append("shellfish");

    const Allergies allergies(properties);

    Json::Value info = allergies.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asString(), "peanuts");
    EXPECT_EQ(info[1].asString(), "pollen");
    EXPECT_EQ(info[2].asString(), "shellfish");
}

TEST(AllergiesTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("latex");
    properties.append("bee stings");

    Allergies allergies;
    allergies.set_info(properties);

    Json::Value info = allergies.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asString(), "latex");
    EXPECT_EQ(info[1].asString(), "bee stings");
}

TEST(AllergiesTest, SetInfoInvalidJson)
{
    const Json::Value properties(Json::objectValue); // Not an array, should trigger exception

    Allergies allergies;
    EXPECT_THROW(allergies.set_info(properties), std::invalid_argument);
}

TEST(AllergiesTest, GetName)
{
    const Allergies allergies;
    EXPECT_EQ(allergies.get_name(), properties::allergies);
}
