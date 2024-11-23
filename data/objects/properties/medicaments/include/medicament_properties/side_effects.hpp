#pragma once

#include <ostream>

#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::medicaments
{
    class SideEffects final : public ArrayProperty<std::string>
    {
    public:
        SideEffects() = default;

        explicit SideEffects(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit SideEffects(std::vector<std::string> side_effects)
            : ArrayProperty(std::move(side_effects))
        {
        }

        ~SideEffects() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::side_effects;
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
