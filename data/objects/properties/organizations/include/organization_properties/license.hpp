#pragma once

#include "organizations/source/properties_definition.hpp"

namespace drug_lib::data::objects::organizations
{
    class License final : public DataProperty
    {
    public:
        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value license_params;
            license_params[names_of_json_fields::license_name] = license_name_;
            license_params[names_of_json_fields::license_key] = license_key_;
            return license_params;
        }

        void set_info(const Json::Value& property) override
        {
            license_name_ = property[names_of_json_fields::license_name].asString();
            license_key_ = property[names_of_json_fields::license_key].asString();
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::license;
        }

        explicit License(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        [[nodiscard]] const std::string& get_license_name() const
        {
            return license_name_;
        }

        void set_license_name(const std::string& license_name)
        {
            license_name_ = license_name;
        }

        [[nodiscard]] const std::string& get_license_key() const
        {
            return license_key_;
        }

        void set_license_key(const std::string& license_key)
        {
            license_key_ = license_key;
        }

    private:
        struct names_of_json_fields
        {
            static constexpr auto license_name = "license_name";
            static constexpr auto license_key = "license_key";
        };

        std::string license_name_;
        std::string license_key_;
    };
}
