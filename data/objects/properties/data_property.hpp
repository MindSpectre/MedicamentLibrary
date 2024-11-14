#pragma once

#include <boost/container/flat_map.hpp>

#include "db_field.hpp"

namespace drug_lib::data
{
    class DataProperty
    {
    public:
        virtual ~DataProperty() = default;

        [[nodiscard]] virtual Json::Value get_info() const = 0;

        virtual void set_info(const Json::Value& property) = 0;

        [[nodiscard]] virtual std::string get_name() const = 0;


        struct _common_properties
        {
        };
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
                result[name] = property->get_info();
            }
            return result;
        }

        [[nodiscard]] std::unique_ptr<common::database::Field<Json::Value>> make_properties_field() const noexcept;

    private:
        boost::container::flat_map<std::string, std::shared_ptr<DataProperty>> m_data;
    };
}
