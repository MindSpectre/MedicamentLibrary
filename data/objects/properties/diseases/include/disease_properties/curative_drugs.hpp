#pragma once

#include <ostream>

#include "diseases/source/properties_definition.hpp"

namespace drug_lib::data::objects::diseases
{
    class CurativeDrugs final : public ArrayProperty<int32_t>
    {
    public:
        CurativeDrugs() = default;

        explicit CurativeDrugs(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit CurativeDrugs(std::vector<int32_t> curative_drugs)
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
