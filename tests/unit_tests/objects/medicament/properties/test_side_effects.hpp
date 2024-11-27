#pragma once
#include <gtest/gtest.h>

#include "medicament_properties/side_effects.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::medicaments;

TEST(SideEffectsTest, DefaultConstructor)
{
    const SideEffects side_effects;
    EXPECT_EQ(side_effects.get_info().size(), 0);
}

TEST(SideEffectsTest, ParameterizedConstructorWithVector)
{
    const std::vector<std::string> names = {"dizzy", "headache", "itch"};
    const SideEffects side_effects(names);

    Json::Value info = side_effects.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asString(), "dizzy");
    EXPECT_EQ(info[1].asString(), "headache");
    EXPECT_EQ(info[2].asString(), "itch");
}

TEST(SideEffectsTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("nausea");
    properties.append("blurred vision");
    properties.append("fatigue");

    const SideEffects side_effects(properties);

    Json::Value info = side_effects.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asString(), "nausea");
    EXPECT_EQ(info[1].asString(), "blurred vision");
    EXPECT_EQ(info[2].asString(), "fatigue");
}

TEST(SideEffectsTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("rash");
    properties.append("insomnia");

    SideEffects side_effects;
    side_effects.set_info(properties);

    Json::Value info = side_effects.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asString(), "rash");
    EXPECT_EQ(info[1].asString(), "insomnia");
}

TEST(SideEffectsTest, SetInfoInvalidJson)
{
    const Json::Value properties(Json::objectValue); // Not an array, should trigger exception

    SideEffects side_effects;
    EXPECT_THROW(side_effects.set_info(properties), std::invalid_argument);
}

TEST(SideEffectsTest, GetName)
{
    const SideEffects side_effects;
    EXPECT_EQ(side_effects.get_name(), properties::side_effects);
}
