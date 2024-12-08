#include <gtest/gtest.h>
#include "organization.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;

TEST(OrganizationTest, DefaultConstructor)
{
    const Organization organization;
    EXPECT_EQ(organization.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(organization.get_name(), "");
    EXPECT_EQ(organization.get_type(), "");
    EXPECT_EQ(organization.get_country(), "");
    EXPECT_EQ(organization.get_contact_details(), "");
}

TEST(OrganizationTest, ParameterizedConstructor)
{
    const Organization organization(common::database::Uuid("1"), "PharmaCorp", "Pharmaceutical", "USA", "123-456-7890");
    EXPECT_EQ(organization.get_id(), "1");
    EXPECT_EQ(organization.get_name(), "PharmaCorp");
    EXPECT_EQ(organization.get_type(), "Pharmaceutical");
    EXPECT_EQ(organization.get_country(), "USA");
    EXPECT_EQ(organization.get_contact_details(), "123-456-7890");
}

TEST(OrganizationTest, SettersAndGetters)
{
    Organization organization;
    organization.set_id("2");
    organization.set_name("MediHealth");
    organization.set_type("Healthcare");
    organization.set_country("UK");
    organization.set_contact_details("987-654-3210");

    EXPECT_EQ(organization.get_id(), "2");
    EXPECT_EQ(organization.get_name(), "MediHealth");
    EXPECT_EQ(organization.get_type(), "Healthcare");
    EXPECT_EQ(organization.get_country(), "UK");
    EXPECT_EQ(organization.get_contact_details(), "987-654-3210");
}

TEST(OrganizationTest, ToRecord)
{
    const Organization organization(common::database::Uuid("3"), "HealthCorp", "Healthcare", "Canada", "555-555-5555");

    for (const auto record = organization.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == Organization::field_name::name)
        {
            EXPECT_EQ("HealthCorp", field->as<std::string>());
        }
        else if (field->get_name() == Organization::field_name::type)
        {
            EXPECT_EQ("Healthcare", field->as<std::string>());
        }
        else if (field->get_name() == Organization::field_name::country)
        {
            EXPECT_EQ("Canada", field->as<std::string>());
        }
        else if (field->get_name() == Organization::field_name::contact_details)
        {
            EXPECT_EQ("555-555-5555", field->as<std::string>());
        }
        else if (field->get_name() == Organization::field_name::id)
        {
            EXPECT_EQ("3", field->as<common::database::Uuid>().get_id());
        }
    }
}

TEST(OrganizationTest, FromRecord)
{
    common::database::Record record;
    record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(Organization::field_name::id, common::database::Uuid("4")));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(Organization::field_name::name, "WellnessCo"));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(Organization::field_name::type, "Wellness"));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(Organization::field_name::country, "Germany"));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(Organization::field_name::contact_details,
                                                               "111-222-3333"));

    Organization organization;
    organization.from_record(record);

    EXPECT_EQ(organization.get_id(), "4");
    EXPECT_EQ(organization.get_name(), "WellnessCo");
    EXPECT_EQ(organization.get_type(), "Wellness");
    EXPECT_EQ(organization.get_country(), "Germany");
    EXPECT_EQ(organization.get_contact_details(), "111-222-3333");
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
