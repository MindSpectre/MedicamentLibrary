#pragma once

#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
    class RiskFactors final : public ArrayProperty<std::string>
    {
    public:
        RiskFactors() = default;

        explicit RiskFactors(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit RiskFactors(std::vector<std::string> risk_factors) :
            ArrayProperty(std::move(risk_factors))
        {
        }

        ~RiskFactors() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::risk_factors;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : data_)
            {
                result.append(ids);
            }
            return result;
        }

        void set_info(const Json::Value& property) override
        {
            if (!property.isArray())
            {
                throw std::invalid_argument("Property is not an array");
            }
            data_.reserve(property.size());
            for (const auto& it : property)
            {
                data_.push_back(it.asString());
            }
        }
    };
}
