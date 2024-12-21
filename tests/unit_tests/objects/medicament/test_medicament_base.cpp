#include <gtest/gtest.h>
#include "medicament.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
TEST(MedicamentTest, DefaultConstructor)
{
    const Medicament medicament;
    EXPECT_EQ(medicament.get_id(), common::database::Uuid::default_value);
    EXPECT_EQ(medicament.get_name(), "");
    EXPECT_EQ(medicament.get_type(), "");
    EXPECT_FALSE(medicament.is_requires_prescription());
}

TEST(MedicamentTest, ParameterizedConstructor)
{
    const Medicament medicament
        (common::database::Uuid("1", true), "Aspirin", "Painkiller", true, "ABUIT123", "accepted", "AV12");
    EXPECT_EQ(medicament.get_id(), "1");
    EXPECT_EQ(medicament.get_name(), "Aspirin");
    EXPECT_EQ(medicament.get_type(), "Painkiller");
    EXPECT_TRUE(medicament.is_requires_prescription());
    EXPECT_EQ(medicament.get_approval_number(), "ABUIT123");
    EXPECT_EQ(medicament.get_approval_status(), "accepted");
    EXPECT_EQ(medicament.get_atc_code(), "AV12");

}

TEST(MedicamentTest, SettersAndGetters)
{
    Medicament medicament;
    medicament.set_id("2");
    medicament.set_name("Paracetamol");
    medicament.set_type("Antipyretic");
    medicament.set_requires_prescription(false);
    medicament.set_approval_number("ABUIT123");
    medicament.set_approval_status("accepted");
    medicament.set_atc_code("AV12");
    EXPECT_EQ(medicament.get_id(), "2");
    EXPECT_EQ(medicament.get_name(), "Paracetamol");
    EXPECT_EQ(medicament.get_type(), "Antipyretic");
    EXPECT_FALSE(medicament.is_requires_prescription());
    EXPECT_EQ(medicament.get_approval_number(), "ABUIT123");
    EXPECT_EQ(medicament.get_approval_status(), "accepted");
    EXPECT_EQ(medicament.get_atc_code(), "AV12");
}

TEST(MedicamentTest, ToRecord)
{
    const Medicament medicament(common::database::Uuid("3", true), "Ibuprofen", "Painkiller", true, "ABUIT123", "accepted",
                                "AV13");

    for (const auto record = medicament.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == medicament::field_name::name)
        {
            EXPECT_EQ("Ibuprofen", field->as<std::string>());
        }
        else if (field->get_name() == medicament::field_name::type)
        {
            EXPECT_EQ("Painkiller", field->as<std::string>());
        }
        else if (field->get_name() == medicament::field_name::requires_prescription)
        {
            EXPECT_TRUE(field->as<bool>());
        }
        else if (field->get_name() == shared::field_name::id)
        {
            EXPECT_EQ("3", field->as<common::database::Uuid>().get_id());
        }
        else if (field->get_name() == medicament::field_name::approval_number)
        {
            EXPECT_EQ("ABUIT123", field->as<std::string>());
        }
        else if (field->get_name() == medicament::field_name::approval_status)
        {
            EXPECT_EQ("accepted", field->as<std::string>());
        }
        else if (field->get_name() == medicament::field_name::atc_code)
        {
            EXPECT_EQ("AV13", field->as<std::string>());
        }
    }
}

TEST(MedicamentTest, FromRecord)
{
    common::database::Record record;
    record.push_back(std::make_unique<common::database::Field<common::database::Uuid>>(shared::field_name::id, common::database::Uuid("4", true)));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(medicament::field_name::name, "Codeine"));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(medicament::field_name::type, "Opioid"));
    record.push_back(
        std::make_unique<common::database::Field<bool>>(medicament::field_name::requires_prescription, true));
   record.push_back(
        std::make_unique<common::database::Field<std::string>>(medicament::field_name::approval_number, "ABUIT123"));
    record.push_back(
        std::make_unique<common::database::Field<std::string>>(medicament::field_name::approval_status, "accepted"));
    record.push_back(std::make_unique<common::database::Field<std::string>>(medicament::field_name::atc_code, "AV14"));
    Medicament medicament;
    medicament.from_record(record);

    EXPECT_EQ(medicament.get_id(), "4");
    EXPECT_EQ(medicament.get_name(), "Codeine");
    EXPECT_EQ(medicament.get_type(), "Opioid");
    EXPECT_TRUE(medicament.is_requires_prescription());
    EXPECT_EQ(medicament.get_approval_number(), "ABUIT123");
    EXPECT_EQ(medicament.get_approval_status(), "accepted");
    EXPECT_EQ(medicament.get_atc_code(), "AV14");
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
