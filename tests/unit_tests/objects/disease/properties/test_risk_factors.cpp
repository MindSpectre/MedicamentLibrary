#include <properties_controller.hpp>
#include <gtest/gtest.h>
#include <json/value.h>

#include "disease_properties/risk_factors.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
using namespace drug_lib::data::objects::diseases;

TEST(RiskFactorsTest, DefaultConstructor)
{
	const RiskFactors risk_factors;
	EXPECT_EQ(risk_factors.get_data().size(), 0);
}

TEST(RiskFactorsTest, ParameterizedConstructor)
{
	const std::vector<std::string> risk_factors_list = {"Smoking", "Obesity", "High blood pressure"};
	const RiskFactors risk_factors(risk_factors_list);
	EXPECT_EQ(risk_factors.get_data().size(), 3);
	EXPECT_EQ(risk_factors.get_data()[0], "Smoking");
	EXPECT_EQ(risk_factors.get_data()[1], "Obesity");
	EXPECT_EQ(risk_factors.get_data()[2], "High blood pressure");
}

TEST(RiskFactorsTest, SetRiskFactors)
{
	RiskFactors risk_factors;
	const std::vector<std::string> risk_factors_list = {"Genetic predisposition", "Sedentary lifestyle"};
	risk_factors.set_data(risk_factors_list);
	EXPECT_EQ(risk_factors.get_data().size(), 2);
	EXPECT_EQ(risk_factors.get_data()[0], "Genetic predisposition");
	EXPECT_EQ(risk_factors.get_data()[1], "Sedentary lifestyle");
}

TEST(RiskFactorsTest, EmplaceBack)
{
	RiskFactors risk_factors;
	risk_factors.emplace_back("Alcohol consumption");
	EXPECT_EQ(risk_factors.get_data().size(), 1);
	EXPECT_EQ(risk_factors.get_data()[0], "Alcohol consumption");
}

TEST(RiskFactorsTest, GetInfo)
{
	const std::vector<std::string> risk_factors_list = {"Poor diet", "Stress"};
	const RiskFactors risk_factors(risk_factors_list);

	Json::Value info = risk_factors.get_info();
	ASSERT_TRUE(info.isArray());
	EXPECT_EQ(info.size(), 2);
	EXPECT_EQ(info[0].asString(), "Poor diet");
	EXPECT_EQ(info[1].asString(), "Stress");
}

TEST(RiskFactorsTest, SetInfo)
{
	Json::Value risk_factors_json(Json::arrayValue);
	risk_factors_json.append("Lack of exercise");
	risk_factors_json.append("Chronic sleep deprivation");

	RiskFactors risk_factors;
	risk_factors.set_info(risk_factors_json);

	EXPECT_EQ(risk_factors.get_data().size(), 2);
	EXPECT_EQ(risk_factors.get_data()[0], "Lack of exercise");
	EXPECT_EQ(risk_factors.get_data()[1], "Chronic sleep deprivation");
}

TEST(RiskFactorsTest, SetInfoInvalidJson)
{
	Json::Value invalid_json(Json::objectValue);
	invalid_json["invalid_field"] = "value";

	RiskFactors risk_factors;
	EXPECT_THROW(risk_factors.set_info(invalid_json), std::invalid_argument);
}

TEST(RiskFactorsTest, FactoryCreate)
{
	const RiskFactors risk_factors;
	EXPECT_NO_THROW(data::PropertyFactory::create<RiskFactors>(risk_factors));
	EXPECT_NO_THROW(data::PropertyFactory::create(diseases::properties::risk_factors, Json::arrayValue));
}
