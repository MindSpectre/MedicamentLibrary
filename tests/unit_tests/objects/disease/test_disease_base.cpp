#include <gtest/gtest.h>
#include "disease.hpp"
#include "diseases/source/properties_definition.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects;

TEST(DiseaseTest, DefaultConstructor)
{
    const Disease disease;
    EXPECT_EQ(disease.get_id(), -1);
    EXPECT_EQ(disease.get_name(), "");
    EXPECT_EQ(disease.get_type(), "");
    EXPECT_FALSE(disease.is_is_infectious());
}

TEST(DiseaseTest, ParameterizedConstructor)
{
    const Disease disease(1, "Flu", "Viral", true);
    EXPECT_EQ(disease.get_id(), 1);
    EXPECT_EQ(disease.get_name(), "Flu");
    EXPECT_EQ(disease.get_type(), "Viral");
    EXPECT_TRUE(disease.is_is_infectious());
}

TEST(DiseaseTest, SettersAndGetters)
{
    Disease disease;
    disease.set_id(2);
    disease.set_name("Cold");
    disease.set_type("Viral");
    disease.set_is_infectious(false);

    EXPECT_EQ(disease.get_id(), 2);
    EXPECT_EQ(disease.get_name(), "Cold");
    EXPECT_EQ(disease.get_type(), "Viral");
    EXPECT_FALSE(disease.is_is_infectious());
}

TEST(DiseaseTest, ToRecord)
{
    const Disease disease(3, "Chickenpox", "Viral", true);
    for (const auto record = disease.to_record().fields(); const auto& field : record)
    {
        if (field->get_name() == Disease::field_name::name)
        {
            EXPECT_EQ("Chickenpox", field->as<std::string>());
        }
        else if (field->get_name() == Disease::field_name::type)
        {
            EXPECT_EQ("Viral", field->as<std::string>());
        }
        else if (field->get_name() == Disease::field_name::is_infectious)
        {
            EXPECT_TRUE(field->as<bool>());
        }
        else if (field->get_name() == Disease::field_name::id)
        {
            EXPECT_EQ(3, field->as<int>());
        }
    }
}

TEST(DiseaseTest, FromRecord)
{
    common::database::Record record;
    record.push_back(std::make_unique<common::database::Field<int32_t>>(Disease::field_name::id, 4));
    record.push_back(std::make_unique<common::database::Field<std::string>>(Disease::field_name::name, "Measles"));
    record.push_back(std::make_unique<common::database::Field<std::string>>(Disease::field_name::type, "Viral"));
    record.push_back(std::make_unique<common::database::Field<bool>>(Disease::field_name::is_infectious, true));

    Disease disease;
    disease.from_record(record);

    EXPECT_EQ(disease.get_id(), 4);
    EXPECT_EQ(disease.get_name(), "Measles");
    EXPECT_EQ(disease.get_type(), "Viral");
    EXPECT_TRUE(disease.is_is_infectious());
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
