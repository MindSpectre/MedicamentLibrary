#include <gtest/gtest.h>
#include <json/json.h>

#include "medicament_properties/strength.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects::medicaments;

TEST(StrengthTest, DefaultConstructor)
{
	const Strength strength;
	EXPECT_EQ(strength.get_description(), "");
}

TEST(StrengthTest, ParameterizedConstructorWithJson)
{
	Json::Value properties;
	properties[Strength::names_of_json_fields::description] =
			"500 mg per tablet";

	const Strength strength(properties);
	EXPECT_EQ(strength.get_description(), "500 mg per tablet");
}

TEST(StrengthTest, ParameterizedConstructorWithString)
{
	const Strength strength("10 mg/mL");

	EXPECT_EQ(strength.get_description(), "10 mg/mL");
}

TEST(StrengthTest, GetInfo)
{
	const Strength strength("250 mg per dose");

	Json::Value info = strength.get_info();
	ASSERT_TRUE(info.isObject());
	EXPECT_EQ(
		info[Strength::names_of_json_fields::description].asString(),
		"250 mg per dose");
}

TEST(StrengthTest, SetInfo)
{
	Json::Value properties;
	properties[Strength::names_of_json_fields::description] =
			"1000 mg per tablet";

	Strength strength;
	strength.set_info(properties);

	EXPECT_EQ(strength.get_description(), "1000 mg per tablet");
}

TEST(StrengthTest, SetDescription)
{
	Strength strength;
	strength.set_description("50 mg per capsule");

	EXPECT_EQ(strength.get_description(), "50 mg per capsule");
}

TEST(StrengthTest, GetName)
{
	const Strength strength;
	EXPECT_EQ(strength.get_name(), properties::strength);
}
