#pragma once

#include <ostream>

#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{
    class CurrentDiseases final : public ArrayProperty<int32_t>
    {
    public:
        CurrentDiseases() = default;

        explicit CurrentDiseases(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit CurrentDiseases(std::vector<int32_t> current_diseases)
            : ArrayProperty(std::move(current_diseases))
        {
        }

        ~CurrentDiseases() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::current_diseases;
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
                data_.push_back(it.asInt());
            }
        }
    };
}
