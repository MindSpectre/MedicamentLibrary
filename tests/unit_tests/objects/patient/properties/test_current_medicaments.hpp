#pragma once

#include <gtest/gtest.h>

#include "patient_properties/current_medicaments.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(CurrentMedicamentsTest, DefaultConstructor)
{
    const CurrentMedicaments current_medicaments;
    EXPECT_EQ(current_medicaments.get_info().size(), 0);
}

TEST(CurrentMedicamentsTest, ParameterizedConstructorWithVector)
{
    const std::vector medicaments = {101, 202, 303};
    const CurrentMedicaments current_medicaments(medicaments);

    Json::Value info = current_medicaments.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asInt(), 101);
    EXPECT_EQ(info[1].asInt(), 202);
    EXPECT_EQ(info[2].asInt(), 303);
}

TEST(CurrentMedicamentsTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append(101);
    properties.append(202);
    properties.append(303);

    const CurrentMedicaments current_medicaments(properties);

    Json::Value info = current_medicaments.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asInt(), 101);
    EXPECT_EQ(info[1].asInt(), 202);
    EXPECT_EQ(info[2].asInt(), 303);
}

TEST(CurrentMedicamentsTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append(404);
    properties.append(505);

    CurrentMedicaments current_medicaments;
    current_medicaments.set_info(properties);

    Json::Value info = current_medicaments.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asInt(), 404);
    EXPECT_EQ(info[1].asInt(), 505);
}

TEST(CurrentMedicamentsTest, SetInfoInvalidJson)
{
    const Json::Value properties(Json::objectValue); // Not an array, should trigger exception

    CurrentMedicaments current_medicaments;
    EXPECT_THROW(current_medicaments.set_info(properties), std::invalid_argument);
}

TEST(CurrentMedicamentsTest, GetName)
{
    const CurrentMedicaments current_medicaments;
    EXPECT_EQ(current_medicaments.get_name(), properties::current_medicaments);
}
