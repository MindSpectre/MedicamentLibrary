#pragma once

#include <ostream>

#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class BloodType final : public DataProperty
    {
    public:
        BloodType() = default;

        explicit BloodType(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit BloodType(std::string type) :
            type_(std::move(type))
        {
        }

        explicit BloodType(const char* type) :
            type_(type)
        {
        }

        ~BloodType() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::blood_type;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result;
            result[names_of_json_fields::blood_type] = type_;
            return result;
        }

        void set_info(const Json::Value& property) override
        {
            type_ = property[names_of_json_fields::blood_type].asString();
        }


        [[nodiscard]] const std::string& get_type() const
        {
            return type_;
        }

        void set_type(std::string type)
        {
            type_ = std::move(type);
        }


        struct names_of_json_fields
        {
            static constexpr std::string blood_type = "blood_type";
        };

        friend std::ostream& operator<<(std::ostream& os, const BloodType& obj)
        {
            return os << " type_: " << obj.type_;
        }

    private:
        // PRESCRIPTION DETAILS
        std::string type_;
    };
}
