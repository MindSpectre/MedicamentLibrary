#pragma once

#include <ostream>

#include "data_property.hpp"
#include "patients/source/properties_definition.hpp"

namespace drug_lib::data::objects::patients
{

    class Vaccines final : public ArrayProperty<std::string>
    {
    public:
        Vaccines() = default;

        explicit Vaccines(const Json::Value& properties)
        {
            this->set_info(properties);
        }

        explicit Vaccines(std::vector<std::string> vaccines) :
            ArrayProperty(std::move(vaccines))
        {
        }

        ~Vaccines() override = default;

        [[nodiscard]] std::string get_name() const override
        {
            return properties::vaccines;
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
