#pragma once
#include <gtest/gtest.h>

#include "disease_properties/affected_age_groups.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
using namespace drug_lib::data::objects::diseases;

TEST(AffectedAgeGroupsTest, DefaultConstructor)
{
    const AffectedAgeGroups age_groups;
    EXPECT_EQ(age_groups.get_data().size(), 0);
}

TEST(AffectedAgeGroupsTest, ParameterizedConstructor)
{
    const std::vector<std::string> age_groups_list = {"Children", "Adults", "Elderly"};
    const AffectedAgeGroups age_groups(age_groups_list);
    EXPECT_EQ(age_groups.get_data().size(), 3);
    EXPECT_EQ(age_groups.get_data()[0], "Children");
    EXPECT_EQ(age_groups.get_data()[1], "Adults");
    EXPECT_EQ(age_groups.get_data()[2], "Elderly");
}

TEST(AffectedAgeGroupsTest, SetAgeGroups)
{
    AffectedAgeGroups age_groups;
    const std::vector<std::string> age_groups_list = {"Infants", "Teenagers"};
    age_groups.set_data(age_groups_list);
    EXPECT_EQ(age_groups.get_data().size(), 2);
    EXPECT_EQ(age_groups.get_data()[0], "Infants");
    EXPECT_EQ(age_groups.get_data()[1], "Teenagers");
}

TEST(AffectedAgeGroupsTest, EmplaceBack)
{
    AffectedAgeGroups age_groups;
    age_groups.emplace_back("Adults");
    EXPECT_EQ(age_groups.get_data().size(), 1);
    EXPECT_EQ(age_groups.get_data()[0], "Adults");
}

TEST(AffectedAgeGroupsTest, GetInfo)
{
    const std::vector<std::string> age_groups_list = {"Children", "Elderly"};
    const AffectedAgeGroups age_groups(age_groups_list);

    Json::Value info = age_groups.get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0].asString(), "Children");
    EXPECT_EQ(info[1].asString(), "Elderly");
}

TEST(AffectedAgeGroupsTest, SetInfo)
{
    Json::Value age_groups_json(Json::arrayValue);
    age_groups_json.append("Infants");
    age_groups_json.append("Teenagers");

    AffectedAgeGroups age_groups;
    age_groups.set_info(age_groups_json);

    EXPECT_EQ(age_groups.get_data().size(), 2);
    EXPECT_EQ(age_groups.get_data()[0], "Infants");
    EXPECT_EQ(age_groups.get_data()[1], "Teenagers");
}

TEST(AffectedAgeGroupsTest, SetInfoInvalidJson)
{
    Json::Value invalid_json(Json::objectValue);
    invalid_json["invalid_field"] = "value";

    AffectedAgeGroups age_groups;
    EXPECT_THROW(age_groups.set_info(invalid_json), std::invalid_argument);
}

TEST(AffectedAgeGroupsTest, FactoryCreate)
{
    const AffectedAgeGroups age_groups;
    EXPECT_NO_THROW(data::PropertyFactory::create<AffectedAgeGroups>(age_groups));
    EXPECT_NO_THROW(data::PropertyFactory::create(diseases::properties::affected_age_groups, Json::arrayValue));
}
