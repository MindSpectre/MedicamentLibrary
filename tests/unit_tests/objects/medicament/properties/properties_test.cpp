#include "medicament.hpp"
#include "properties_controller.hpp"
#include "test_prescription.hpp"
#include "test_active_ingredients.hpp"
#include "test_inactive_ingredients.hpp"
#include "test_dosage_form.hpp"
#include "test_side_effects.hpp"
#include "test_strength.hpp"
#include <gtest/gtest.h>

using namespace drug_lib;
using namespace drug_lib::data::objects;
TEST(MedicamentPropertiesTest, AddProperty)
{
    Medicament Medicament;
    const auto symptoms = data::PropertyFactory::create<medicaments::Prescription>(Json::Value());
    Medicament.add_property(symptoms);

    const auto retrieved_property = Medicament.get_property(medicaments::properties::prescription);
    ASSERT_NE(retrieved_property, nullptr);
    EXPECT_EQ(retrieved_property->get_name(), medicaments::properties::prescription);
}

TEST(MedicamentPropertiesTest, RemoveProperty)
{
    Medicament Medicament;
    const auto symptoms = data::PropertyFactory::create<medicaments::Prescription>(Json::Value());
    Medicament.add_property(symptoms);

    Medicament.remove_property(medicaments::properties::prescription);
    EXPECT_THROW(auto t = Medicament.get_property(medicaments::properties::prescription),
                 boost::container::out_of_range);
}

TEST(MedicamentPropertiesTest, GetPropertyInfo)
{
    Medicament Medicament;
    medicaments::Prescription prescription("Test descriptionInfo");
    const auto prescription_ = data::PropertyFactory::create<medicaments::Prescription>(std::move(prescription));

    Medicament.add_property(prescription_);
    auto sym_list = Medicament.get_property(medicaments::properties::prescription);
    Json::Value info = prescription_->get_info();
    ASSERT_TRUE(info.isObject());
    EXPECT_EQ(info[medicaments::Prescription::names_of_json_fields::description], "Test descriptionInfo");
}

TEST(MedicamentPropertiesTest, ValidationJson)
{
    Medicament Medicament(common::database::Uuid("3"), "Aspirin", "Painkiller", false, "ABUIT123", "accepted", "AV13");
    medicaments::Prescription prescription("Test descriptionInfo");
    const auto prescription_ = data::PropertyFactory::create<medicaments::Prescription>(std::move(prescription));

    Medicament.add_property(prescription_);
    for (const auto record = Medicament.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == shared::field_name::properties)
        {
            const auto info = field->as<Json::Value>();
            ASSERT_TRUE(info.isObject());
            EXPECT_TRUE(info.isMember(medicaments::properties::prescription));
            EXPECT_EQ(info[medicaments::properties::prescription]
                      [medicaments::Prescription::names_of_json_fields::description]
                      .asString(),
                      prescription.get_description());
        }
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
