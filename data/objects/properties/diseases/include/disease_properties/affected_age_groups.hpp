#pragma once

#include <ostream>
#include "data_property.hpp"
#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
    class AffectedAgeGroups final : public ArrayProperty<std::string>
    {
    public:
        AffectedAgeGroups() = default;

        explicit AffectedAgeGroups(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit AffectedAgeGroups(std::vector<std::string> affected_age_groups) :
            ArrayProperty(std::move(affected_age_groups))
        {
        }

        ~AffectedAgeGroups() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::affected_age_groups;
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
