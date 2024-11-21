
#pragma once

#include "medicament_properties.hpp"
#include "organization_properties.hpp"
#include "patient_properties.hpp"
#include "diseases_properties.hpp"

namespace drug_lib::data
{
    class PropertyFactory final
    {
    public:
        template <typename T>
            requires std::is_constructible_v<T, Json::Value>
        static std::shared_ptr<DataProperty> create(const Json::Value& property_value)
        {
            return std::make_shared<T>(property_value);
        }

        template <typename T, typename... Args>
        static std::shared_ptr<DataProperty> create(Args&&... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

        static std::shared_ptr<DataProperty> create(const std::string& property_name,
                                                    const Json::Value& property_value)
        {
            if (property_name == objects::medicaments::properties::prescription)
            {
                return std::make_shared<objects::medicaments::PrescriptionDrug>(property_value);
            }
            if (property_name == objects::diseases::properties::symptoms)
            {
                return std::make_shared<objects::diseases::Symptoms>(property_value);
            }
            if (property_name == objects::organizations::properties::license)
            {
                return std::make_shared<objects::organizations::License>(property_value);
            }
            if (property_name == objects::patients::properties::current_diseases)
            {
                return std::make_shared<objects::patients::CurrentDiseases>(property_value);
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

            void create_collection(const std::unique_ptr<common::database::FieldBase>& field)
            {
                auto values = field->as<Json::Value>();
                for (Json::Value::const_iterator it = values.begin(); it != values.end(); ++it)
                {
                    collection_.add_property(PropertyFactory::create(it.name(), *it));
                }
            }

            void create_collection(const std::string& field)
            {
                const Json::CharReaderBuilder builder;
                Json::Value values;
                std::istringstream s(field);
                std::string errors;
                parseFromStream(builder, s, &values, &errors);
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