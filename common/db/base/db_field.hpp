#pragma once

#include <chrono>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <json/json.h>
#include <json/value.h>

namespace drug_lib::common::database
{
    template <typename T>
    class Field;
    /// @brief Base class representing a field in the database

    class FieldBase
    {
    public:
        virtual ~FieldBase() = default;

        /// @return Column name of the field
        [[nodiscard]] virtual const std::string& get_name() const = 0;

        /// @brief Converts the field value to a string for SQL queries
        [[nodiscard]] virtual std::string to_string() const & = 0;

        /// @brief Converts the field value to a string for SQL queries
        /// @return moveable
        [[nodiscard]] virtual std::string to_string() && = 0;

        /// @brief Gets the SQL data type of the field
        [[nodiscard]] virtual std::string get_sql_type() const = 0;

        template <typename T>
        T as() const
        {
            // Ensure that T is a valid type, e.g., int, std::string, etc.
            static_assert(std::is_default_constructible_v<T>, "T must be default constructible");

            // Attempt to dynamic cast this object to a Field<T> type

            if (const auto* derivedField = dynamic_cast<const Field<T>*>(this))
            {
                return derivedField->value();
            }
            throw std::runtime_error("FieldBase::as(): Incorrect type requested for field " + get_name());
        }

        [[nodiscard]] virtual std::unique_ptr<FieldBase> clone() const = 0;
    };

    /// @brief Represents a field of a specific type in the database
    /// @tparam T Type of the value in the database
    template <typename T>
    class Field final : public FieldBase
    {
    public:
        Field(std::string name, T value)
            : name_(std::move(name)), value_(std::move(value))
        {
        }

        /// @return Column name of the field
        [[nodiscard]] const std::string& get_name() const override { return name_; }

        void set_name(const std::string& name) { name_ = name; }
        void set_name(std::string&& name) { name_ = std::move(name); }

        /// @return Value of the field
        const T& value() const { return value_; }
        void set_value(const T& value) { value_ = value; }
        void set_value(T&& value) { value_ = std::move(value); }

        /// @brief Converts the field value to a string for SQL queries
        [[nodiscard]] std::string to_string() const & override
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                return value_;
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return value_ ? "TRUE" : "FALSE";
            }
            else if constexpr (std::is_arithmetic_v<T>)
            {
                return std::to_string(value_);
            }
            else if constexpr (std::is_same_v<T, Json::Value>)
            {
                return value_.toStyledString();
            }
            else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
            {
                // Convert time_point to string in ISO 8601 format
                const std::time_t time = std::chrono::system_clock::to_time_t(value_);
                const std::tm* tm_ptr = std::localtime(&time);
                std::ostringstream oss;
                oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
                return oss.str();
            }
            else
            {
                static_assert(sizeof(T) == 0, "Unsupported field type for to_string()");
            }
            return {};
        }

        /// @brief Converts the field value to a string for SQL queries
        [[nodiscard]] std::string to_string() && override
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                return std::move(value_); // Move the string instead of copying
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return value_ ? "TRUE" : "FALSE";
            }
            else if constexpr (std::is_arithmetic_v<T>)
            {
                return std::to_string(value_); // No need to move arithmetic types
            }
            else if constexpr (std::is_same_v<T, Json::Value>)
            {
                return value_.toStyledString(); // Move the JSON string representation
            }
            else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
            {
                // Convert time_point to string in ISO 8601 format
                const std::time_t time = std::chrono::system_clock::to_time_t(value_);
                const std::tm* tm_ptr = std::localtime(&time);
                std::ostringstream oss;
                oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M:%S");
                return oss.str(); // No need to move, it's a local object
            }
            else
            {
                static_assert(sizeof(T) == 0, "Unsupported field type for to_string()");
            }
            return {};
        }

        /// @brief Gets the SQL data type of the field
        [[nodiscard]] std::string get_sql_type() const override
        {
            if constexpr (std::is_same_v<T, int> || std::is_same_v<T, int32_t>)
            {
                return "INT";
            }
            else if constexpr (std::is_same_v<T, int64_t>)
            {
                return "BIGINT";
            }
            else if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>)
            {
                return "DOUBLE PRECISION";
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return "TEXT";
            }
            else if constexpr (std::is_same_v<T, Json::Value>)
            {
                return "JSONB";
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                return "BOOLEAN";
            }
            else if constexpr (std::is_same_v<T, std::chrono::system_clock::time_point>)
            {
                return "TIMESTAMP";
            }
            else
            {
                static_assert(sizeof(T) == 0, "Unsupported field type for get_sql_type()");
            }
            return {};
        }

        [[nodiscard]] std::unique_ptr<FieldBase> clone() const override
        {
            return std::make_unique<Field>(*this);
        }

    private:
        std::string name_;
        T value_;
    };

    class ViewingField final : public FieldBase
    {
    public:
        ViewingField(std::string&& name, std::string_view&& value)
            : name_(std::move(name)), value_(value)
        {
        }

        /// @return Column name of the field
        [[nodiscard]] const std::string& get_name() const override { return name_; }

        void set_name(const std::string& name) { name_ = name; }
        void set_name(std::string&& name) { name_ = std::move(name); }

        /// @return Value of the field
        [[nodiscard]] std::string_view value() const { return value_; }
        void set_value(const std::string_view& value) { value_ = value; }
        void set_value(std::string_view&& value) { value_ = value; }

        /// @brief Converts the field value to a string for SQL queries
        [[nodiscard]] std::string to_string() const & override
        {
            return std::string(value_);
        }

        /// @brief Converts the field value to a string for SQL queries
        [[nodiscard]] std::string to_string() && override
        {
            return std::string(value_);
        }

        /// @brief Gets the SQL data type of the field
        [[nodiscard]] std::string get_sql_type() const override
        {
            throw std::runtime_error("get_sql_type() called in VIEWING field");
        }

        [[nodiscard]] std::unique_ptr<FieldBase> clone() const override
        {
            return std::make_unique<ViewingField>(*this);
        }

    private:
        std::string name_;
        std::string_view value_;
    };
}
