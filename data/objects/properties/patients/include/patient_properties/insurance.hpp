#pragma once

#include <ostream>
#include "data_property.hpp"
#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class Insurance final : public DataProperty
    {
    public:
        Insurance() = default;

        explicit Insurance(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit Insurance(std::string insurance) :
            insurance_(std::move(insurance))
        {
        }

        explicit Insurance(const char* insurance) :
            insurance_(insurance)
        {
        }

        ~Insurance() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::insurance;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result;
            result[names_of_json_fields::insurance] = insurance_;
            return result;
        }

        void set_info(const Json::Value& property) override
        {
            insurance_ = property[names_of_json_fields::insurance].asString();
        }


        [[nodiscard]] const std::string& get_insurance() const
        {
            return insurance_;
        }

        void set_insurance(std::string insurance)
        {
            insurance_ = std::move(insurance);
        }


        struct names_of_json_fields
        {
            static constexpr std::string insurance = "insurance";
        };

        friend std::ostream& operator<<(std::ostream& os, const Insurance& obj)
        {
            return os << " identification: " << obj.insurance_;
        }

    private:
        // PRESCRIPTION DETAILS
        std::string insurance_;
    };
}
