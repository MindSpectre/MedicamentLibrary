#pragma once
#include "db_record.hpp"
#include "property_factory.hpp"

namespace drug_lib::data::objects
{
    class ObjectBase
    {
    public:
        virtual ~ObjectBase() = default;
        [[nodiscard]] virtual common::database::Record to_record() const = 0;
        virtual void from_record(const common::database::Record&) = 0;
    };

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
