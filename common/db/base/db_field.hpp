#pragma once

#include <string>
namespace drug_lib::data
{
    /// @brief base class displying field in database
    class FieldBase
    {
    public:
        virtual ~FieldBase() = default;
        /// @return column name of element
        virtual const std::string &name() const = 0;
    };
    /// @brief real field of database
    /// @tparam T type of value in database
    template <typename T>
    class Field final: public FieldBase 
    {
    public:
        Field(std::string name, T value)
            : name_(std::move(name)), value_(std::move(value)) {}

        const std::string &name() const override { return name_; }

        const T &value() const { return value_; }
        void set_value(const T &value) { value_ = value; }
        void set_value(T &&value) { value_ = std::move(value); }

    private:
        std::string name_;
        T value_;
    };
}
