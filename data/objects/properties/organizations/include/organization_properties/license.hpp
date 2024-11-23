#pragma once

#include <ostream>

#include "organizations/source/properties_definition.hpp"

namespace drug_lib::data::objects::organizations
{
    class License final : public DataProperty
    {
    public:
        License() = default;

        explicit License(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        License(std::string license_name, std::string license_key)
            : license_name_(std::move(license_name)),
              license_key_(std::move(license_key))
        {
        }

        ~License() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::license;
        }

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

        struct names_of_json_fields
        {
            static constexpr auto license_name = "license_name";
            static constexpr auto license_key = "license_key";
        };

        friend std::ostream& operator<<(std::ostream& os, const License& obj)
        {
            return os
                << "license_name_: " << obj.license_name_
                << " license_key_: " << obj.license_key_;
        }

    private:
        std::string license_name_;
        std::string license_key_;
    };
}
