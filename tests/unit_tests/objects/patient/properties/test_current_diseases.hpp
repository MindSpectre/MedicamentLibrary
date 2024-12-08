#pragma once

#include <gtest/gtest.h>

#include "patient_properties/current_diseases.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(CurrentDiseasesTest, DefaultConstructor)
{
	const CurrentDiseases current_diseases;
	EXPECT_EQ(current_diseases.get_info().size(), 0);
}

TEST(CurrentDiseasesTest, ParameterizedConstructorWithVector)
{
	const std::vector<common::database::Uuid> diseases = {
		common::database::Uuid("101"),
		common::database::Uuid("202")
	};
	const CurrentDiseases current_diseases(diseases);

	Json::Value info = current_diseases.get_info();
	ASSERT_TRUE(info.isArray());
	EXPECT_EQ(info.size(), 2);
	EXPECT_EQ(info[0].asString(), "101");
	EXPECT_EQ(info[1].asString(), "202");
}

TEST(CurrentDiseasesTest, ParameterizedConstructorWithJson)
{
	Json::Value properties(Json::arrayValue);
	properties.append("101");
	properties.append("202");
	properties.append("303");

	const CurrentDiseases current_diseases(properties);

	Json::Value info = current_diseases.get_info();
	ASSERT_TRUE(info.isArray());
	EXPECT_EQ(info.size(), 3);
	EXPECT_EQ(info[0].asString(), "101");
	EXPECT_EQ(info[1].asString(), "202");
	EXPECT_EQ(info[2].asString(), "303");
}

TEST(CurrentDiseasesTest, SetInfoValidJson)
{
	Json::Value properties(Json::arrayValue);
	properties.append("404");
	properties.append("505");

	CurrentDiseases current_diseases;
	current_diseases.set_info(properties);

	Json::Value info = current_diseases.get_info();
	ASSERT_TRUE(info.isArray());
	EXPECT_EQ(info.size(), 2);
	EXPECT_EQ(info[0].asString(), "404");
	EXPECT_EQ(info[1].asString(), "505");
}

TEST(CurrentDiseasesTest, SetInfoInvalidJson)
{
	const Json::Value properties(Json::objectValue); // Not an array, should trigger exception

	CurrentDiseases current_diseases;
	EXPECT_THROW(current_diseases.set_info(properties), std::invalid_argument);
}

TEST(CurrentDiseasesTest, GetName)
{
	const CurrentDiseases current_diseases;
	EXPECT_EQ(current_diseases.get_name(), properties::current_diseases);
}
