#pragma once

#include <ostream>

#include "medicaments/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class Prescription final : public DataProperty
    {
    public:
        Prescription() = default;

        explicit Prescription(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit Prescription(std::string description)
            : description_(std::move(description))
        {
        }

        explicit Prescription(const char* description) : description_(std::move(description))
        {
        }

        ~Prescription() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::prescription;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result;
            result[names_of_json_fields::description] = description_;
            return result;
        }

        void set_info(const Json::Value& property) override
        {
            description_ = property[names_of_json_fields::description].asString();
        }


        [[nodiscard]] const std::string& get_description() const
        {
            return description_;
        }

        void set_description(std::string description)
        {
            description_ = std::move(description);
        }


        struct names_of_json_fields
        {
            static constexpr std::string description = "description";
        };

        friend std::ostream& operator<<(std::ostream& os, const Prescription& obj)
        {
            return os << " description_: " << obj.description_;
        }

    private:
        // PRESCRIPTION DETAILS
        std::string description_;
    };
}
