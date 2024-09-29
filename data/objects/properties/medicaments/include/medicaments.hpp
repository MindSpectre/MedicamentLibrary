#pragma once

#include "data_property.hpp"

namespace drug_lib::data::objects::medicaments
{
    class PrescriptionDrug final : public DataProperty
    {
    public:
        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result;
            result.append(prescription_);
            return result;
        }

        void set_info(const Json::Value& val) override
        {
            prescription_ = val[properties::prescription].asString();
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::prescription;
        }


        PrescriptionDrug() = default;

        explicit PrescriptionDrug(const Json::Value& properties)
        {
            this->set_info(properties);
        }

    private:
        // PRESCRIPTION DETAILS
        std::string prescription_;
    };
}
