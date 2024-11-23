#pragma once

#include <gtest/gtest.h>

#include "disease_properties/curative_drugs.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::diseases;

TEST(CurativeDrugsTest, DefaultConstructor)
{
    const CurativeDrugs curative_drugs;
    EXPECT_EQ(curative_drugs.get_info().size(), 0);
}

TEST(CurativeDrugsTest, ParameterizedConstructorWithVector)
{
    const std::vector diseases = {101, 202, 303};
    const CurativeDrugs curative_drugs(diseases);

    Json::Value info = curative_drugs.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asInt(), 101);
    EXPECT_EQ(info[1].asInt(), 202);
    EXPECT_EQ(info[2].asInt(), 303);
}

TEST(CurativeDrugsTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append(101);
    properties.append(202);
    properties.append(303);

    const CurativeDrugs curative_drugs(properties);

    Json::Value info = curative_drugs.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
    EXPECT_EQ(info[0].asInt(), 101);
    EXPECT_EQ(info[1].asInt(), 202);
    EXPECT_EQ(info[2].asInt(), 303);
}

TEST(CurativeDrugsTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append(404);
    properties.append(505);

    CurativeDrugs curative_drugs;
    curative_drugs.set_info(properties);

    Json::Value info = curative_drugs.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asInt(), 404);
    EXPECT_EQ(info[1].asInt(), 505);
}

TEST(CurativeDrugsTest, SetInfoInvalidJson)
{
    const Json::Value properties(Json::objectValue); // Not an array, should trigger exception

    CurativeDrugs curative_drugs;
    EXPECT_THROW(curative_drugs.set_info(properties), std::invalid_argument);
}

TEST(CurativeDrugsTest, GetName)
{
    const CurativeDrugs curative_drugs;
    EXPECT_EQ(curative_drugs.get_name(), properties::curative_drugs);
}

TEST(CurativeDrugsTest, FactoryCreate)
{
    const CurativeDrugs curative_drugs;
    EXPECT_NO_THROW(data::PropertyFactory::create<CurativeDrugs>(curative_drugs));
    EXPECT_NO_THROW(data::PropertyFactory::create(diseases::properties::curative_drugs, Json::arrayValue));
}
