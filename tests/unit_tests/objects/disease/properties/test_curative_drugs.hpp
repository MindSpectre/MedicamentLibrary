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
    const std::vector diseases = {drug_lib::common::database::Uuid(), drug_lib::common::database::Uuid(),drug_lib::common::database::Uuid()};
    const CurativeDrugs curative_drugs(diseases);

    Json::Value info = curative_drugs.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
}

TEST(CurativeDrugsTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("default");
    properties.append("default");
    properties.append("default");

    const CurativeDrugs curative_drugs(properties);

    Json::Value info = curative_drugs.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 3);
}

TEST(CurativeDrugsTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    properties.append("default");
    properties.append("default");

    CurativeDrugs curative_drugs;
    curative_drugs.set_info(properties);

    Json::Value info = curative_drugs.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asString(), "default");
    EXPECT_EQ(info[1].asString(), "default");
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
