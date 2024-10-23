// db_conditions.hpp

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "db_field.hpp"

namespace drug_lib::common::database
{
    class FieldCondition
    {
    public:
        FieldCondition(std::unique_ptr<FieldBase> field, std::string op, std::unique_ptr<FieldBase> value)
            : field_(std::move(field)), operator_(std::move(op)), value_(std::move(value))
        {
        }

        [[nodiscard]] const std::unique_ptr<FieldBase>& field() const { return field_; }
        [[nodiscard]] const std::string& op() const { return operator_; }
        [[nodiscard]] const std::unique_ptr<FieldBase>& value() const { return value_; }

    private:
        std::unique_ptr<FieldBase> field_;
        std::string operator_;
        std::unique_ptr<FieldBase> value_;
    };

    class FieldConditions final
    {
    public:
        ~FieldConditions() = default;

        void add_condition(FieldCondition&& condition)
        {
            conditions_.push_back(std::move(condition));
        }

        [[nodiscard]] const std::vector<FieldCondition>& conditions() const &
        {
            return conditions_;
        }

        // Implement begin(), end(), empty(), and size() methods
        auto begin() { return conditions_.begin(); }
        auto end() { return conditions_.end(); }
        [[nodiscard]] auto begin() const { return conditions_.cbegin(); }
        [[nodiscard]] auto end() const { return conditions_.cend(); }
        [[nodiscard]] bool empty() const { return conditions_.empty(); }
        [[nodiscard]] size_t size() const { return conditions_.size(); }

    private:
        std::vector<FieldCondition> conditions_;
    };
}
