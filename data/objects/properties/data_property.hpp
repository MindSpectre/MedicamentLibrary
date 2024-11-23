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

    template <typename DataPropertyType>
    class ArrayProperty : public DataProperty
    {
    public:
        ~ArrayProperty() override = default;

        explicit ArrayProperty(std::vector<DataPropertyType> data_v)
            : data_(std::move(data_v))
        {
        }

        ArrayProperty() = default;

        [[nodiscard]] const std::vector<DataPropertyType>& get_data() const
        {
            return data_;
        }

        void set_data(std::vector<DataPropertyType> data)
        {
            data_ = std::move(data);
        }

        template <typename... Args>
        ArrayProperty& emplace_back(Args&&... args)
        {
            data_.emplace_back(std::forward<Args>(args)...);
            return *this;
        }

        template <typename TypeC>
            requires std::is_same_v<std::remove_cvref_t<TypeC>, DataPropertyType>
        void push_back(TypeC&& element)
        {
            data_.push_back(std::forward<TypeC>(element));
        }

        void pop_back()
        {
            data_.pop_back();
        }

        DataPropertyType operator[](const std::size_t index) const
        {
            if (index >= data_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return data_[index];
        }

        DataPropertyType& operator[](const std::size_t index)
        {
            if (index >= data_.size())
            {
                throw std::out_of_range("index out of range");
            }
            return data_[index];
        }

        friend std::ostream& operator<<(std::ostream& os, const ArrayProperty& obj)
        {
            os << "[";
            for (std::size_t idx = 0; idx < obj.data_.size(); ++idx)
            {
                os << obj.data_[idx];
                if (idx < obj.data_.size() - 1)
                {
                    os << ", ";
                }
            }
            os << "]";
            return os;
        }

    protected:
        std::vector<DataPropertyType> data_;
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
