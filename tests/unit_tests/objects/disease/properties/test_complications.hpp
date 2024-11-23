#pragma once
#include <gtest/gtest.h>

#include "disease_properties/complications.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
using namespace drug_lib::data::objects::diseases;

TEST(ComplicationsTest, DefaultConstructor)
{
    const Complications complications;
    EXPECT_EQ(complications.get_data().size(), 0);
}

TEST(ComplicationsTest, ParameterizedConstructor)
{
    const std::vector<std::string> complications_list = {"Pneumonia", "Organ failure", "Sepsis"};
    const Complications complications(complications_list);
    EXPECT_EQ(complications.get_data().size(), 3);
    EXPECT_EQ(complications.get_data()[0], "Pneumonia");
    EXPECT_EQ(complications.get_data()[1], "Organ failure");
    EXPECT_EQ(complications.get_data()[2], "Sepsis");
}

TEST(ComplicationsTest, SetComplications)
{
    Complications complications;
    const std::vector<std::string> complications_list = {"Chronic fatigue", "Heart disease"};
    complications.set_data(complications_list);
    EXPECT_EQ(complications.get_data().size(), 2);
    EXPECT_EQ(complications.get_data()[0], "Chronic fatigue");
    EXPECT_EQ(complications.get_data()[1], "Heart disease");
}

TEST(ComplicationsTest, EmplaceBack)
{
    Complications complications;
    complications.emplace_back("Kidney failure");
    EXPECT_EQ(complications.get_data().size(), 1);
    EXPECT_EQ(complications.get_data()[0], "Kidney failure");
}

TEST(ComplicationsTest, GetInfo)
{
    const std::vector<std::string> complications_list = {"Lung damage", "Stroke"};
    const Complications complications(complications_list);

    Json::Value info = complications.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asString(), "Lung damage");
    EXPECT_EQ(info[1].asString(), "Stroke");
}

TEST(ComplicationsTest, SetInfo)
{
    Json::Value complications_json(Json::arrayValue);
    complications_json.append("Hearing loss");
    complications_json.append("Vision impairment");

    Complications complications;
    complications.set_info(complications_json);

    EXPECT_EQ(complications.get_data().size(), 2);
    EXPECT_EQ(complications.get_data()[0], "Hearing loss");
    EXPECT_EQ(complications.get_data()[1], "Vision impairment");
}

TEST(ComplicationsTest, SetInfoInvalidJson)
{
    Json::Value invalid_json(Json::objectValue);
    invalid_json["invalid_field"] = "value";

    Complications complications;
    EXPECT_THROW(complications.set_info(invalid_json), std::invalid_argument);
}

TEST(ComplicationsTest, FactoryCreate)
{
    const Complications complications;
    EXPECT_NO_THROW(data::PropertyFactory::create<Complications>(complications));
    EXPECT_NO_THROW(data::PropertyFactory::create(diseases::properties::complications, Json::arrayValue));
}
