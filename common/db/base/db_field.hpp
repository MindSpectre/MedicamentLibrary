#pragma once

#include <string>
#include <type_traits>
#include <memory>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace drug_lib::common::database
{
    /// @brief Base class representing a field in the database
    class FieldBase
    {
    public:
        virtual ~FieldBase() = default;

        /// @return Column name of the field
        [[nodiscard]] virtual const std::string& get_name() const = 0;

        /// @brief Converts the field value to a string for SQL queries
        [[nodiscard]] virtual std::string to_string() const = 0;

        /// @brief Gets the SQL data type of the field
        [[nodiscard]] virtual std::string get_sql_type() const = 0;

        template <typename T>
        T as() const;
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
        [[nodiscard]] std::string to_string() const override
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

    private:
        std::string name_;
        T value_;
    };

    template <typename T>
    T FieldBase::as() const
    {
        // Ensure that T is a valid type, e.g., int, std::string, etc.
        static_assert(std::is_default_constructible_v<T>, "T must be default constructible");

        // Attempt to dynamic cast this object to a Field<T> type
        const auto* derived = std::dynamic_pointer_cast<const Field<T>>(this);

        if (!derived)
        {
            throw std::runtime_error("FieldBase::as(): Incorrect type requested for field " + get_name());
        }

        return derived->value();
    }
}
