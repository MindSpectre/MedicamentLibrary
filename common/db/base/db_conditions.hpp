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

        [[nodiscard]] const std::unique_ptr<FieldBase>& field() const & { return field_; }
        [[nodiscard]] const std::string& op() const & { return operator_; }
        [[nodiscard]] const std::unique_ptr<FieldBase>& value() const & { return value_; }

    private:
        std::unique_ptr<FieldBase> field_;
        std::string operator_;
        std::unique_ptr<FieldBase> value_;
    };

    class PatternCondition final
    {
    public:
        explicit PatternCondition(std::string&& pattern) : pattern_(std::move(pattern))
        {
        }

        explicit PatternCondition(const std::string& pattern) : pattern_(pattern)
        {
        }

        ~PatternCondition() = default;

        [[nodiscard]] const std::string& get_pattern() const &
        {
            return pattern_;
        }

        void set_pattern(std::string&& pattern) &
        {
            pattern_ = std::move(pattern);
        }

        void set_pattern(const std::string& pattern) &
        {
            pattern_ = pattern;
        }

    protected:
        std::string pattern_;
    };

    class Conditions final
    {
    public:
        ~Conditions() = default;

        void add_field_condition(FieldCondition&& condition) &
        {
            conditions_.push_back(std::move(condition));
        }

        void add_pattern_condition(PatternCondition&& condition) &
        {
            patterns_.push_back(std::move(condition));
        }

        void pop_field_condition() &
        {
            conditions_.pop_back();
        }

        void pop_pattern_condition() &
        {
            patterns_.pop_back();
        }

        void clear_field_conditions() &
        {
            conditions_.clear();
        }

        void clear_pattern_conditions() &
        {
            patterns_.clear();
        }

        [[nodiscard]] const std::vector<FieldCondition>& fields_conditions() const &
        {
            return conditions_;
        }

        [[nodiscard]] const std::vector<PatternCondition>& pattern_conditions() const &
        {
            return patterns_;
        }

        [[nodiscard]] bool empty() const
        {
            return conditions_.empty() && patterns_.empty();
        }

        [[nodiscard]] size_t size() const { return conditions_.size(); }

    private:
        std::vector<FieldCondition> conditions_;
        std::vector<PatternCondition> patterns_;
    };
}
