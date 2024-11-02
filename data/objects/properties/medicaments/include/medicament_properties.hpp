#pragma once

#include "data_property.hpp"

namespace drug_lib::data::objects::medicaments
{
    struct attributes : DataProperty::_cm_attributes
    {
        static constexpr std::string prescription = "prescription";
    };

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
            prescription_ = val[attributes::prescription].asString();
        }

        [[nodiscard]] std::string get_name() const override
        {
            return attributes::prescription;
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
