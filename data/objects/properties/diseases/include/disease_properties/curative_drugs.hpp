#pragma once

#include <ostream>
#include "data_property.hpp"
#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
    class CurativeDrugs final : public ArrayProperty<common::database::Uuid>
    {
    public:
        CurativeDrugs() = default;

        explicit CurativeDrugs(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit CurativeDrugs(std::vector<common::database::Uuid> curative_drugs)
            : ArrayProperty(std::move(curative_drugs))
        {
        }

        ~CurativeDrugs() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::curative_drugs;
        }

        [[nodiscard]] Json::Value get_info() const override
        {
            Json::Value result(Json::arrayValue);
            for (const auto& ids : data_)
            {
                result.append(ids.get_id());
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
                data_.emplace_back(std::move(it.asString()), false);
            }
        }
    };
}
