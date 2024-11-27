#include <gtest/gtest.h>
#include "organization.hpp"
#include "properties_controller.hpp"
#include "test_license.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;

TEST(OrganizationPropertiesTest, AddProperty)
{
    Organization organization;
    const auto license = data::PropertyFactory::create<organizations::License>(Json::Value());
    organization.add_property(license);

    const auto retrieved_property = organization.get_property(organizations::properties::license);
    ASSERT_NE(retrieved_property, nullptr);
    EXPECT_EQ(retrieved_property->get_name(), organizations::properties::license);
}

TEST(OrganizationPropertiesTest, RemoveProperty)
{
    Organization organization;
    const auto license = data::PropertyFactory::create<organizations::License>(Json::Value());
    organization.add_property(license);

    organization.remove_property(organizations::properties::license);
    EXPECT_THROW(auto t = organization.get_property(organizations::properties::license),
                 boost::container::out_of_range);
}

TEST(OrganizationPropertiesTest, GetPropertyInfo)
{
    Organization organization;
    const auto license = data::PropertyFactory::create<organizations::License>("1", "2");
    organization.add_property(license);
    auto sym_list = organization.get_property(organizations::properties::license);
    Json::Value info = license->get_info();
    ASSERT_TRUE(info.isObject());
    EXPECT_EQ(info[organizations::License::names_of_json_fields::license_key].asString(), "2");
}

TEST(OrganizationPropertiesTest, ValidationJson)
{
    Organization organization(1, "SAM", "TYPE", "US", "111");

    const auto license = data::PropertyFactory::create<organizations::License>("1", "2");
    organization.add_property(license);
    for (const auto record = organization.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == Organization::field_name::properties)
        {
            const auto info = field->as<Json::Value>();
            ASSERT_TRUE(info.isObject());
            EXPECT_TRUE(info.isMember(organizations::properties::license));
            EXPECT_EQ(
                info[organizations::properties::license][organizations::License::names_of_json_fields::license_name]
                .asString(), "1");
            EXPECT_EQ(
                info[organizations::properties::license][organizations::License::names_of_json_fields::license_key].
                asString(), "2");
        }
    }
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
