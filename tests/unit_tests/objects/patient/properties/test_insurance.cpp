#include <gtest/gtest.h>
#include <json/value.h>

#include "patient_properties/insurance.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(InsuranceTest, DefaultConstructor)
{
	const Insurance insurance;
	EXPECT_EQ(insurance.get_insurance(), ""); // Default should be an empty string
}

TEST(InsuranceTest, ParameterizedConstructorWithString)
{
	const Insurance insurance("HealthSecure123");
	EXPECT_EQ(insurance.get_insurance(), "HealthSecure123");
}

TEST(InsuranceTest, ParameterizedConstructorWithCharPointer)
{
	const Insurance insurance("PremiumPlan");
	EXPECT_EQ(insurance.get_insurance(), "PremiumPlan");
}

TEST(InsuranceTest, ParameterizedConstructorWithJson)
{
	Json::Value properties;
	properties[Insurance::names_of_json_fields::insurance] = "StandardCare";

	const Insurance insurance(properties);
	EXPECT_EQ(insurance.get_insurance(), "StandardCare");
}

TEST(InsuranceTest, SetInfoValidJson)
{
	Json::Value properties;
	properties[Insurance::names_of_json_fields::insurance] = "BasicPlan";

	Insurance insurance;
	insurance.set_info(properties);

	EXPECT_EQ(insurance.get_insurance(), "BasicPlan");
}

TEST(InsuranceTest, SetInfoInvalidJson)
{
	const Json::Value properties; // Missing "insurance" field

	Insurance insurance;
	EXPECT_NO_THROW(insurance.set_info(properties)); // Should not throw, but handle gracefully
	EXPECT_EQ(insurance.get_insurance(), ""); // Expect empty string as default
}

TEST(InsuranceTest, GetInfo)
{
	const Insurance insurance("SilverPlan");
	Json::Value info = insurance.get_info();

	ASSERT_TRUE(info.isObject());
	EXPECT_EQ(info[Insurance::names_of_json_fields::insurance].asString(), "SilverPlan");
}

TEST(InsuranceTest, GetName)
{
	const Insurance insurance;
	EXPECT_EQ(insurance.get_name(), properties::insurance);
}

TEST(InsuranceTest, SetInsurance)
{
	Insurance insurance;
	insurance.set_insurance("GoldPlan");
	EXPECT_EQ(insurance.get_insurance(), "GoldPlan");
}

TEST(InsuranceTest, StreamOperator)
{
	const Insurance insurance("PlatinumCare");
	std::ostringstream os;
	os << insurance;

	EXPECT_EQ(os.str(), " identification: PlatinumCare");
}
