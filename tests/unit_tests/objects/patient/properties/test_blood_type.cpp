#include <gtest/gtest.h>
#include <json/value.h>

#include "patient_properties/blood_type.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(BloodTypeTest, DefaultConstructor)
{
    const BloodType blood_type;
    EXPECT_EQ(blood_type.get_type(), ""); // Default should be an empty string
}

TEST(BloodTypeTest, ParameterizedConstructorWithString)
{
    const BloodType blood_type("A+");
    EXPECT_EQ(blood_type.get_type(), "A+");
}

TEST(BloodTypeTest, ParameterizedConstructorWithCharPointer)
{
    const BloodType blood_type("O-");
    EXPECT_EQ(blood_type.get_type(), "O-");
}

TEST(BloodTypeTest, ParameterizedConstructorWithJson)
{
    Json::Value properties;
    properties[BloodType::names_of_json_fields::blood_type] = "B+";

    const BloodType blood_type(properties);
    EXPECT_EQ(blood_type.get_type(), "B+");
}

TEST(BloodTypeTest, SetInfoValidJson)
{
    Json::Value properties;
    properties[BloodType::names_of_json_fields::blood_type] = "AB-";

    BloodType blood_type;
    blood_type.set_info(properties);

    EXPECT_EQ(blood_type.get_type(), "AB-");
}

TEST(BloodTypeTest, SetInfoInvalidJson)
{
    const Json::Value properties; // Missing "blood_type" field

    BloodType blood_type;
    EXPECT_NO_THROW(blood_type.set_info(properties)); // Should not throw, but handle gracefully
    EXPECT_EQ(blood_type.get_type(), ""); // Expect empty string as default
}

TEST(BloodTypeTest, GetInfo)
{
    const BloodType blood_type("O+");
    Json::Value info = blood_type.get_info();

    ASSERT_TRUE(info.isObject());
    EXPECT_EQ(info[BloodType::names_of_json_fields::blood_type].asString(), "O+");
}

TEST(BloodTypeTest, GetName)
{
    const BloodType blood_type;
    EXPECT_EQ(blood_type.get_name(), properties::blood_type);
}

TEST(BloodTypeTest, SetType)
{
    BloodType blood_type;
    blood_type.set_type("A-");
    EXPECT_EQ(blood_type.get_type(), "A-");
}

TEST(BloodTypeTest, StreamOperator)
{
    const BloodType blood_type("B+");
    std::ostringstream os;
    os << blood_type;

    EXPECT_EQ(os.str(), " type_: B+");
}
