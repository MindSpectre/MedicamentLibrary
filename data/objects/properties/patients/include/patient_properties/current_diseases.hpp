#pragma once

#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class CurrentDiseases final : public DataProperty
    {
    public:
        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : current_diseases_)
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
            current_diseases_.resize(property.size());
            for (const auto& it : property)
            {
                current_diseases_.push_back(it.asInt());
            }
        }

        [[nodiscard]] std::string get_name() const override
        {
            return properties::current_diseases;
        }

    private:
        std::vector<int32_t> current_diseases_;
    };
}
