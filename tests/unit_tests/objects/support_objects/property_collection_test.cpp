#include <gtest/gtest.h>
#include "objects.hpp"

namespace drug_lib::data
{
    TEST(PropertyCollectionTest, AddProperty)
    {
        PropertyCollection collection;
        objects::organizations::License license;
        license.set_license_name("Sample License");
        license.set_license_key("LICENSE123");
        objects::diseases::Symptoms symptoms;
        symptoms.set_symptoms({});
        collection.add_property(PropertyFactory::create<objects::organizations::License>(license));
        collection.add_property(PropertyFactory::create<objects::diseases::Symptoms>(symptoms));
        EXPECT_NO_THROW(auto v = collection.get_property(objects::organizations::properties::license));
        EXPECT_NO_THROW(auto v = collection.get_property(objects::diseases::properties::symptoms));
    }

    TEST(PropertyCollectionTest, RemovePropertyByPointer)
    {
        PropertyCollection collection;
        const auto license = PropertyFactory::create<objects::organizations::License>();
        collection.add_property(license);

        collection.remove_property(license);
        EXPECT_THROW(auto v = collection.get_property("license"), boost::container::out_of_range);
    }

    TEST(PropertyCollectionTest, RemovePropertyByName)
    {
        PropertyCollection collection;
        collection.add_property(PropertyFactory::create<objects::organizations::License>());
        collection.remove_property(objects::organizations::properties::license);
        EXPECT_THROW(auto v = collection.get_property("license"), boost::container::out_of_range);
    }

    TEST(PropertyCollectionTest, GetProperty)
    {
        PropertyCollection collection;
        const auto license = std::make_shared<objects::organizations::License>(Json::Value());
        license->set_license_name("Test License");
        collection.add_property(license);

        const auto retrieved = collection.get_property("license");
        const auto retrieved_license = std::dynamic_pointer_cast<objects::organizations::License>(retrieved);
        ASSERT_NE(retrieved_license, nullptr);
        EXPECT_EQ(retrieved_license->get_license_name(), "Test License");
    }

    TEST(PropertyCollectionTest, GetCollectionInfo)
    {
        PropertyCollection collection;
        objects::organizations::License license1;
        license1.set_license_name("License One");
        license1.set_license_key("KEY1");
        objects::organizations::License license2;
        license2.set_license_name("License Two");
        license2.set_license_key("KEY2");


        collection.add_property(PropertyFactory::create<objects::organizations::License>(license2));
        collection.add_property(PropertyFactory::create<objects::organizations::License>(license1));
        // Overwrite license 2
        Json::Value info = collection.get_collection_info();
        std::cout << info.toStyledString() << std::endl;

        EXPECT_EQ(info.size(), 1);
        EXPECT_TRUE(info.isMember("license"));
        EXPECT_EQ(info["license"][objects::organizations::License::names_of_json_fields::license_name].asString(),
                  "License One");
        EXPECT_EQ(info["license"][objects::organizations::License::names_of_json_fields::license_key].asString(),
                  "KEY1");
    }
}
