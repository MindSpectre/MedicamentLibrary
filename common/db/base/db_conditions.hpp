// db_conditions.hpp

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "db_field.hpp"

namespace drug_lib::common::database
{
    class FieldCondition
    {
    public:
        ~FieldCondition() = default;
        FieldCondition(FieldCondition&&) noexcept = default;
        FieldCondition& operator=(FieldCondition&&) noexcept = default;
        FieldCondition(const FieldCondition&) = delete;
        FieldCondition& operator=(const FieldCondition&) = delete;

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

    enum class order_type
    {
        ascending,
        descending
    };

    class OrderCondition final
    {
    public:
        ~OrderCondition() = default;
        OrderCondition(OrderCondition&&) noexcept = default;
        OrderCondition& operator=(OrderCondition&&) noexcept = default;
        OrderCondition(const OrderCondition&) = delete;
        OrderCondition& operator=(const OrderCondition&) = delete;

        explicit OrderCondition(std::unique_ptr<FieldBase>&& field,
                                const order_type order = order_type::ascending, std::string spec_ = {}) :
            column_(std::move(field)), order_(order), specifier_(std::move(spec_))
        {
        }

        explicit OrderCondition(std::string column_name,
                                const order_type order = order_type::ascending, std::string spec_ = {}) :
            order_(order), specifier_(std::move(spec_))
        {
            column_ = std::make_unique<Field<int32_t>>(std::move(column_name), 0);
        }

        [[nodiscard]] const std::unique_ptr<FieldBase>& get_column() const
        {
            return column_;
        }

        void set_column(std::unique_ptr<FieldBase>&& column) &
        {
            column_ = std::move(column);
        }

        [[nodiscard]] order_type get_order() const
        {
            return order_;
        }

        void set_order(const order_type order) &
        {
            order_ = order;
        }

        [[nodiscard]] const std::string& get_specifier() const
        {
            return specifier_;
        }

        void set_specifier(const std::string& specifier) &
        {
            specifier_ = specifier;
        }

        void set_specifier(std::string&& specifier) &
        {
            specifier_ = std::move(specifier);
        }

    private:
        std::unique_ptr<FieldBase> column_;
        order_type order_ = order_type::ascending;
        std::string specifier_;
    };

    class PageCondition final
    {
    public:
        ~PageCondition() = default;
        // Constructors
        explicit PageCondition(const int32_t limit, const int32_t offset = 0)
            : limit_(limit), offset_(offset)
        {
        }

        // Accessors
        [[nodiscard]] uint32_t get_limit() const &
        {
            return limit_;
        }

        [[nodiscard]] uint32_t get_offset() const &
        {
            return offset_;
        }

        // Modifiers
        void set_limit(const uint32_t limit) &
        {
            limit_ = limit;
        }

        PageCondition& set_limit(const uint32_t limit) &&
        {
            limit_ = limit;
            return *this;
        }

        /// @warning accept 1-indexing
        PageCondition& set_page_number(const uint32_t page_number) &&
        {
            offset_ = limit_ * (page_number - 1);
            return *this;
        }

        void set_page_number(const int32_t page_number) &
        {
            offset_ = limit_ * (page_number - 1);
        }

        void set_offset(const uint32_t offset)
        {
            offset_ = offset;
        }

    private:
        uint32_t limit_; // Number of records per page
        uint32_t offset_; // Starting point in the dataset (calculated as page_number * limit)
    };

    class Conditions final
    {
    public:
        ~Conditions() = default;

        void add_field_condition(FieldCondition&& condition) &
        {
            conditions_.push_back(std::move(condition));
        }

        template <typename... Args>
        void add_field_condition(Args&&... args) &
        {
            conditions_.emplace_back(std::forward<Args>(args)...);
        }

        void add_pattern_condition(PatternCondition&& condition) &
        {
            patterns_.push_back(std::move(condition));
        }

        template <typename... Args>
        void add_pattern_condition(Args&&... args) &
        {
            patterns_.emplace_back(std::forward<Args>(args)...);
        }

        void add_order_by_condition(OrderCondition&& condition) &
        {
            orders_.push_back(std::move(condition));
        }

        template <typename... Args>
        void add_order_by_condition(Args&&... args) &
        {
            orders_.emplace_back(std::forward<Args>(args)...);
        }


        template <typename... Args>
        void set_page_condition(Args&&... args) &
        {
            pages_ = PageCondition(std::forward<Args>(args)...);
        }


        void set_page_condition(PageCondition condition) &
        {
            pages_ = condition;
        }

        void pop_field_condition() &
        {
            conditions_.pop_back();
        }

        void pop_pattern_condition() &
        {
            patterns_.pop_back();
        }

        void pop_order_by_condition() &
        {
            orders_.pop_back();
        }

        void clear_field_conditions() &
        {
            conditions_.clear();
        }

        void clear_pattern_conditions() &
        {
            patterns_.clear();
        }

        void clear_order_by_conditions() &
        {
            orders_.clear();
        }

        void clear_page_conditions() &
        {
            pages_.reset();
        }

        [[nodiscard]] const std::vector<FieldCondition>& fields_conditions() const &
        {
            return conditions_;
        }

        [[nodiscard]] const std::vector<PatternCondition>& pattern_conditions() const &
        {
            return patterns_;
        }

        [[nodiscard]] const std::vector<OrderCondition>& order_by_conditions() const &
        {
            return orders_;
        }

        [[nodiscard]] const std::optional<PageCondition>& page_condition() const &
        {
            return pages_;
        }

        [[nodiscard]] bool empty() const
        {
            return conditions_.empty() && patterns_.empty() && orders_.empty() && !pages_.has_value();
        }

    private:
        std::vector<FieldCondition> conditions_;
        std::vector<PatternCondition> patterns_;
        std::vector<OrderCondition> orders_;
        std::optional<PageCondition> pages_;
    };
}
