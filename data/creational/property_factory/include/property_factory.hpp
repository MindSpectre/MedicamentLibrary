
#pragma once

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

    namespace objects
    {
        class PropertiesHolder
        {
        protected:
            PropertyCollection collection_;

            void create_collection(const std::shared_ptr<common::database::FieldBase>& field)
            {
                auto values = field->as<Json::Value>();
                for (Json::Value::const_iterator it = values.begin(); it != values.end(); ++it)
                {
                    collection_.add_property(PropertyFactory::create(it.name(), *it));
                }
            }

        public:
            virtual ~PropertiesHolder() = default;

            void add_property(const std::shared_ptr<DataProperty>& prop)
            {
                collection_.add_property(prop);
            }

            [[nodiscard]] std::shared_ptr<DataProperty> get_property(const std::string& name) const
            {
                return collection_.get_property(name);
            }

            void remove_property(const std::string& name)
            {
                collection_.remove_property(name);
            }
        };
    }
}
