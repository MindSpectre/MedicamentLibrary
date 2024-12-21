#include <gtest/gtest.h>
#include "patient.hpp"
#include "properties_controller.hpp"
#include "test_current_diseases.hpp"
#include "test_current_medicaments.hpp"
#include "test_allergies.hpp"
#include "test_blood_type.hpp"
#include "test_insurance.hpp"
#include "test_medical_history.hpp"
#include "test_vaccines.hpp"
using namespace drug_lib;
using namespace drug_lib::data::objects;
TEST(PatientPropertiesTest, AddProperty)
{
    Patient patient;
    const auto current_diseases = data::PropertyFactory::create<patients::CurrentDiseases>(
        Json::Value(Json::arrayValue));
    patient.add_property(current_diseases);

    const auto retrieved_property = patient.get_property(patients::properties::current_diseases);
    ASSERT_NE(retrieved_property, nullptr);
    EXPECT_EQ(retrieved_property->get_name(), patients::properties::current_diseases);
}

TEST(PatientPropertiesTest, RemoveProperty)
{
    Patient patient;
    const auto current_diseases = data::PropertyFactory::create<CurrentDiseases>(std::vector<common::database::Uuid>{
        common::database::Uuid("1"), common::database::Uuid("2"), common::database::Uuid("3")
    });
    patient.add_property(current_diseases);

    patient.remove_property(patients::properties::current_diseases);
    EXPECT_THROW(auto t = patient.get_property(patients::properties::current_diseases),
                 boost::container::out_of_range);
}

TEST(PatientPropertiesTest, GetPropertyInfo)
{
    Patient patient;
    std::vector<common::database::Uuid> current_diseases_list = {common::database::Uuid("1"), common::database::Uuid("2")};
    const auto current_diseases = data::PropertyFactory::create<patients::CurrentDiseases>(
        current_diseases_list);

    patient.add_property(current_diseases);
    Json::Value info = patient.get_property(patients::properties::current_diseases)->get_info();
    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info[0], current_diseases_list[0].get_id());
}

TEST(PatientPropertiesTest, ValidationJson)
{
    Patient patient;
    std::vector<common::database::Uuid> current_diseases_list = {common::database::Uuid("1"), common::database::Uuid("2")};
    const auto current_diseases = data::PropertyFactory::create<patients::CurrentDiseases>(
        current_diseases_list);

    patient.add_property(current_diseases);
    for (const auto record = patient.to_record().fields();
         const auto &field: record)
    {
        if (field->get_name() == shared::field_name::properties)
        {
            const auto info = field->as<Json::Value>();
            ASSERT_TRUE(info.isObject());
            EXPECT_TRUE(info.isMember(patients::properties::current_diseases));
            EXPECT_TRUE(info[patients::properties::current_diseases].isArray());
            for (uint32_t i = 0; i < current_diseases_list.size(); ++i)
            {
                EXPECT_EQ(info[patients::properties::current_diseases][i].asString(), current_diseases_list[i].get_id());
            }
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
