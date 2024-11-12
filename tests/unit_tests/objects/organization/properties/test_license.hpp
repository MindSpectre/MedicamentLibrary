#pragma once
#include "organization_properties/license.hpp"
#include <gtest/gtest.h>

namespace drug_lib::data::objects::organizations
{
    TEST(LicenseTest, DefaultConstructor)
    {
        const License license;
        EXPECT_EQ(license.get_license_name(), "");
        EXPECT_EQ(license.get_license_key(), "");
    }

    TEST(LicenseTest, ParameterizedConstructor)
    {
        Json::Value properties;
        properties[License::names_of_json_fields::license_name] = "Medical License";
        properties[License::names_of_json_fields::license_key] = "ABC123";

        const License license(properties);
        EXPECT_EQ(license.get_license_name(), "Medical License");
        EXPECT_EQ(license.get_license_key(), "ABC123");
    }

    TEST(LicenseTest, SetLicenseName)
    {
        License license;
        license.set_license_name("Updated License");
        EXPECT_EQ(license.get_license_name(), "Updated License");
    }

    TEST(LicenseTest, SetLicenseKey)
    {
        License license;
        license.set_license_key("XYZ789");
        EXPECT_EQ(license.get_license_key(), "XYZ789");
    }

    TEST(LicenseTest, GetInfo)
    {
        License license;
        license.set_license_name("Sample License");
        license.set_license_key("SAMPLE123");

        Json::Value info = license.get_info();
        ASSERT_TRUE(info.isObject());
        EXPECT_EQ(info[License::names_of_json_fields::license_name].asString(), "Sample License");
        EXPECT_EQ(info[License::names_of_json_fields::license_key].asString(), "SAMPLE123");
    }

    TEST(LicenseTest, SetInfo)
    {
        Json::Value properties;
        properties[License::names_of_json_fields::license_name] = "New License";
        properties[License::names_of_json_fields::license_key] = "NEW_KEY456";

        License license;
        license.set_info(properties);

        EXPECT_EQ(license.get_license_name(), "New License");
        EXPECT_EQ(license.get_license_key(), "NEW_KEY456");

        license.set_info(Json::Value());

        EXPECT_EQ(license.get_license_name(), "");
        EXPECT_EQ(license.get_license_key(), "");
    }
}
