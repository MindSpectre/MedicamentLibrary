#pragma once

#include <gtest/gtest.h>
#include "patient_properties/medical_history.hpp"

using namespace drug_lib;
using namespace drug_lib::data::objects::patients;

TEST(MedicalHistoryTest, DefaultConstructor)
{
    const MedicalHistory history;
    EXPECT_EQ(history.get_info().size(), 0);
}

TEST(MedicalHistoryTest, ParameterizedConstructorWithVector)
{
    auto start_date = std::chrono::year{2022} / std::chrono::month{1};
    auto end_date = std::chrono::year{2022} / std::chrono::month{12};
    std::vector<HealthRecord> records = {
        {101, start_date, end_date},
        {102, start_date},
    };

    const MedicalHistory history(records);
    EXPECT_EQ(history.get_info().size(), 2);
}

TEST(MedicalHistoryTest, ParameterizedConstructorWithJson)
{
    Json::Value properties(Json::arrayValue);
    Json::Value record1, record2;

    record1[HealthRecord::names_of_json_fields::disease_id] = 101;
    record1[HealthRecord::names_of_json_fields::start_date] = "2022-01";
    record1[HealthRecord::names_of_json_fields::end_date] = "2022-12";

    record2[HealthRecord::names_of_json_fields::disease_id] = 102;
    record2[HealthRecord::names_of_json_fields::start_date] = "2023-03";

    properties.append(record1);
    properties.append(record2);

    const MedicalHistory history(properties);
    EXPECT_EQ(history.get_info().size(), 2);
}

TEST(MedicalHistoryTest, SetInfoValidJson)
{
    Json::Value properties(Json::arrayValue);
    Json::Value record1, record2;

    record1[HealthRecord::names_of_json_fields::disease_id] = 103;
    record1[HealthRecord::names_of_json_fields::start_date] = "2022-05";
    record1[HealthRecord::names_of_json_fields::end_date] = "2023-01";

    record2[HealthRecord::names_of_json_fields::disease_id] = 104;
    record2[HealthRecord::names_of_json_fields::start_date] = "2023-06";

    properties.append(record1);
    properties.append(record2);

    MedicalHistory history;
    history.set_info(properties);

    EXPECT_EQ(history.get_info().size(), 2);
}

TEST(MedicalHistoryTest, SetInfoInvalidJson)
{
    Json::Value invalid_property(Json::objectValue); // Not an array

    MedicalHistory history;
    EXPECT_THROW(history.set_info(invalid_property), std::invalid_argument);
}

TEST(MedicalHistoryTest, GetInfo)
{
    auto start_date = std::chrono::year{2022} / std::chrono::month{1};
    auto end_date = std::chrono::year{2022} / std::chrono::month{12};
    std::vector<HealthRecord> records = {
        {101, start_date, end_date},
        {102, start_date},
    };

    const MedicalHistory history(records);
    Json::Value info = history.get_info();

    ASSERT_TRUE(info.isArray());
    EXPECT_EQ(info.size(), 2);
    EXPECT_EQ(info[0][HealthRecord::names_of_json_fields::disease_id].asInt(), 101);
    EXPECT_EQ(info[0][HealthRecord::names_of_json_fields::start_date].asString(), "2022-01");
    EXPECT_EQ(info[0][HealthRecord::names_of_json_fields::end_date].asString(), "2022-12");
    EXPECT_EQ(info[1][HealthRecord::names_of_json_fields::disease_id].asInt(), 102);
    EXPECT_EQ(info[1][HealthRecord::names_of_json_fields::start_date].asString(), "2022-01");
    EXPECT_EQ(info[1][HealthRecord::names_of_json_fields::end_date].asString(), "N/A");
}
