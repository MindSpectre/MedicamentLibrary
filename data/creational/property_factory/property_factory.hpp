
#pragma once

#include "data_property.hpp"
#include "medicaments.hpp"

namespace drug_lib::data
{
    class PropertyFactory final
    {
    public:
        template <typename T>
        static std::shared_ptr<DataProperty> create()
        {
            return std::make_shared<T>();
        }

        static std::shared_ptr<DataProperty> create(const std::string& property_name,
                                                    const Json::Value& property_value = Json::Value())
        {
            if (property_name == properties::prescription)
            {
                return std::make_shared<objects::medicaments::PrescriptionDrug>(property_value);
            }
            throw std::invalid_argument("Property '" + property_name + "' not found");
        }
    };
}
