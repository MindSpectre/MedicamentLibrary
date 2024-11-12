#include <gtest/gtest.h>
#include "medicament.hpp"

namespace drug_lib::data::objects
{
    TEST(MedicamentTest, DefaultConstructor)
    {
        const Medicament medicament;
        EXPECT_EQ(medicament.id(), -1);
        EXPECT_EQ(medicament.get_name(), "");
        EXPECT_EQ(medicament.get_type(), "");
        EXPECT_FALSE(medicament.is_requires_prescription());
        EXPECT_EQ(medicament.get_manufacturer_id(), -1);
    }

    TEST(MedicamentTest, ParameterizedConstructor)
    {
        const Medicament medicament(1, "Aspirin", "Painkiller", true, 1001);
        EXPECT_EQ(medicament.id(), 1);
        EXPECT_EQ(medicament.get_name(), "Aspirin");
        EXPECT_EQ(medicament.get_type(), "Painkiller");
        EXPECT_TRUE(medicament.is_requires_prescription());
        EXPECT_EQ(medicament.get_manufacturer_id(), 1001);
    }

    TEST(MedicamentTest, SettersAndGetters)
    {
        Medicament medicament;
        medicament.set_id(2);
        medicament.set_name("Paracetamol");
        medicament.set_type("Antipyretic");
        medicament.set_requires_prescription(false);
        medicament.set_manufacturer_id(1002);

        EXPECT_EQ(medicament.id(), 2);
        EXPECT_EQ(medicament.get_name(), "Paracetamol");
        EXPECT_EQ(medicament.get_type(), "Antipyretic");
        EXPECT_FALSE(medicament.is_requires_prescription());
        EXPECT_EQ(medicament.get_manufacturer_id(), 1002);
    }

    TEST(MedicamentTest, ToRecord)
    {
        const Medicament medicament(3, "Ibuprofen", "Painkiller", true, 1003);

        for (const auto record = medicament.to_record().fields(); const auto& field : record)
        {
            if (field->get_name() == Medicament::field_name::name)
            {
                EXPECT_EQ("Ibuprofen", field->as<std::string>());
            }
            else if (field->get_name() == Medicament::field_name::type)
            {
                EXPECT_EQ("Painkiller", field->as<std::string>());
            }
            else if (field->get_name() == Medicament::field_name::requires_prescription)
            {
                EXPECT_TRUE(field->as<bool>());
            }
            else if (field->get_name() == Medicament::field_name::manufacturer_id)
            {
                EXPECT_EQ(1003, field->as<int>());
            }
            else if (field->get_name() == Medicament::field_name::id)
            {
                EXPECT_EQ(3, field->as<int>());
            }
        }
    }

    TEST(MedicamentTest, FromRecord)
    {
        common::database::Record record;
        record.push_back(std::make_unique<common::database::Field<int32_t>>(Medicament::field_name::id, 4));
        record.push_back(
            std::make_unique<common::database::Field<std::string>>(Medicament::field_name::name, "Codeine"));
        record.push_back(
            std::make_unique<common::database::Field<std::string>>(Medicament::field_name::type, "Opioid"));
        record.push_back(
            std::make_unique<common::database::Field<bool>>(Medicament::field_name::requires_prescription, true));
        record.push_back(
            std::make_unique<common::database::Field<int32_t>>(Medicament::field_name::manufacturer_id, 1004));

        Medicament medicament;
        medicament.from_record(record);

        EXPECT_EQ(medicament.id(), 4);
        EXPECT_EQ(medicament.get_name(), "Codeine");
        EXPECT_EQ(medicament.get_type(), "Opioid");
        EXPECT_TRUE(medicament.is_requires_prescription());
        EXPECT_EQ(medicament.get_manufacturer_id(), 1004);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
