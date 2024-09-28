#pragma once

#include <db_field.hpp>
#include <json/json.h>
#include <boost/container/flat_map.hpp>

#include "../../common/db/exceptions/exceptions.hpp"

namespace drug_lib::data
{
    class DataProperty
    {
    protected:
        std::string name_;
        bool status_ = false;

    public:
        virtual ~DataProperty() = default;
        [[nodiscard]] virtual Json::Value get_info() const = 0;

        [[nodiscard]] std::string get_name() const
        {
            return name_;
        }

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
            return std::make_shared<common::database::Field<Json::Value>>("properties", result);
        }

    private:
        boost::container::flat_map<std::string, std::shared_ptr<DataProperty>> m_data;
    };

    class PropertyFactoryRegister final
    {
    };

    class PropertyFactory final
    {
    public:
        template <typename T>
        static std::shared_ptr<DataProperty> create()
        {
            return std::make_shared<T>();
        }
    };
}

