#pragma once

#include <db_field.hpp>
#include <boost/container/flat_map.hpp>
#include <json/json.h>


namespace drug_lib::data
{
    namespace properties
    {
        const std::string properties = "properties";
        const std::string prescription = "prescription";
        const std::string def_property = "def_property";
    }

    class DataProperty
    {
    protected:
        bool status_ = false;

    public:
        virtual ~DataProperty() = default;


        [[nodiscard]] virtual Json::Value get_info() const = 0;

        virtual void set_info(const Json::Value&) = 0;

        [[nodiscard]] virtual std::string get_name() const = 0;


        void enable()
        {
            status_ = true;
        }

        void disable()
        {
            status_ = false;
        }

        [[nodiscard]] bool enabled() const
        {
            return status_;
        }
    };

    class PropertyCollection final
    {
    public:
        void add_property(const std::shared_ptr<DataProperty>& property)
        {
            m_data[property->get_name()] = property;
        }

        void remove_property(const std::shared_ptr<DataProperty>& property)
        {
            m_data.erase(property->get_name());
        }

        void remove_property(const std::string& property_name)
        {
            m_data.erase(property_name);
        }

        [[nodiscard]] std::shared_ptr<DataProperty> get_property(const std::string& property_name) const
        {
            return m_data.at(property_name);
        }

        std::shared_ptr<DataProperty> operator[](const std::string& property_name) const
        {
            return m_data.at(property_name);
        }

        [[nodiscard]] Json::Value get_collection_info() const noexcept
        {
            Json::Value result;
            for (const auto& [name, property] : m_data)
            {
                if (property->enabled())
                {
                    result[name] = property->get_info();
                }
            }
            return result;
        }

        [[nodiscard]] std::shared_ptr<common::database::Field<Json::Value>> make_properties_field() const noexcept
        {
            Json::Value result;
            for (const auto& [name, property] : m_data)
            {
                if (property->enabled())
                {
                    result[name] = property->get_info();
                }
            }
            return std::make_shared<common::database::Field<Json::Value>>(properties::properties, result);
        }

    private:
        boost::container::flat_map<std::string, std::shared_ptr<DataProperty>> m_data;
    };
}
