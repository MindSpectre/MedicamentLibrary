#include <gtest/gtest.h>
#include "disease.hpp"
#include "medicament.hpp"
#include "patient.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;

TEST(PatientTest, DefaultConstructor)
{
    const Patient patient;
    EXPECT_EQ(patient.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(patient.get_name(), "");
    EXPECT_EQ(patient.get_gender(), "");
    EXPECT_EQ(patient.get_contact_information(), "");
}

TEST(PatientTest, ParameterizedConstructor)
{
    const Patient patient(common::database::Uuid("4", true), "John Doe", "Male",
                          std::chrono::year_month_day{
                              std::chrono::year{1990}, std::chrono::month{5}, std::chrono::day{15}
                          }, "123-456-7890");
    EXPECT_EQ(patient.get_id(), "4");
    EXPECT_EQ(patient.get_name(), "John Doe");
    EXPECT_EQ(patient.get_gender(), "Male");
    EXPECT_EQ(patient.get_contact_information(), "123-456-7890");
}

TEST(PatientTest, SettersAndGetters)
{
    Patient patient;
    patient.set_id("2");
    patient.set_name("Jane Doe");
    patient.set_gender("Female");
    patient.set_birth_date(std::chrono::year_month_day{
        std::chrono::year{1985}, std::chrono::month{8}, std::chrono::day{25}
    });
    patient.set_contact_information("987-654-3210");

    EXPECT_EQ(patient.get_id(), "2");
    EXPECT_EQ(patient.get_name(), "Jane Doe");
    EXPECT_EQ(patient.get_gender(), "Female");
    EXPECT_EQ(patient.get_contact_information(), "987-654-3210");
}

TEST(PatientTest, ToRecord)
{
    const Patient patient(common::database::Uuid("3", true), "Alice Smith", "Female", std::chrono::year_month_day{
                              std::chrono::year{2000}, std::chrono::month{1}, std::chrono::day{1}
                          }, "555-555-5555");

    for (const auto record = patient.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == patient::field_name::personal_name)
        {
            EXPECT_EQ("Alice Smith", field->as<std::string>());
        }
        else if (field->get_name() == patient::field_name::gender)
        {
            EXPECT_EQ("Female", field->as<std::string>());
        }
        else if (field->get_name() == patient::field_name::contact_information)
        {
            EXPECT_EQ("555-555-5555", field->as<std::string>());
        }
        else if (field->get_name() == shared::field_name::id)
        {
            EXPECT_EQ("3", field->as<common::database::Uuid>().get_id());
        }
    }
}

TEST(PatientTest, FromRecord)
{
    common::database::Record record;
    record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(shared::field_name::id, common::database::Uuid("4", true)));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(patient::field_name::personal_name, "Bob Brown"));
    record.push_back(std::make_unique<common::database::Field<std::string>>(patient::field_name::gender, "Male"));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(patient::field_name::contact_information,
                                                               "111-222-3333"));

    Patient patient;
    patient.from_record(record);

    EXPECT_EQ(patient.get_id(), "4");
    EXPECT_EQ(patient.get_name(), "Bob Brown");
    EXPECT_EQ(patient.get_gender(), "Male");
    EXPECT_EQ(patient.get_contact_information(), "111-222-3333");
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
