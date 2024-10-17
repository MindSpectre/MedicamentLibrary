// db_record.hpp

#pragma once
#include <list>
#include <memory>
#include <string>
#include <boost/container/flat_map.hpp>

#include "db_field.hpp"

namespace drug_lib::common::database
{
    class Record final
    {
    public:
        ~Record() = default;

        void push_back(std::unique_ptr<FieldBase>&& field)
        {
            fields_.push_back(std::move(field));
        }

        void pop_back()
        {
            fields_.pop_back();
        }

        void clear()
        {
            fields_.clear();
        }

        Record(Record&& other) noexcept
            : fields_(std::move(other.fields_))
        {
        }

        // Explicitly define the move assignment operator
        Record& operator=(Record&& other) noexcept
        {
            if (this != &other)
            {
                fields_ = std::move(other.fields_);
            }
            return *this;
        }

        // Disable the copy constructor and assignment operator to avoid accidental copying
        Record(const Record&) = delete;
        Record& operator=(const Record&) = delete;
        /*explicit*/
        Record() = default;

        [[nodiscard]] std::unique_ptr<FieldBase> pull_back()
        {
            auto tmp = std::move(fields_.back());
            fields_.pop_back();
            return std::move(tmp);
        }

        [[nodiscard]] std::size_t size() const
        {
            return fields_.size();
        }

        const std::unique_ptr<FieldBase>& operator[](const std::size_t idx) const &
        {
            return fields_[idx];
        }


        [[nodiscard]] bool empty() const
        {
            return fields_.empty();
        }

        [[nodiscard]] const std::vector<std::unique_ptr<FieldBase>>& fields() const &
        {
            return fields_;
        }

        [[nodiscard]] std::vector<std::unique_ptr<FieldBase>> fields() &&
        {
            return std::move(fields_);
        }


        // Implement begin() and end() methods
        auto begin() { return fields_.begin(); }
        auto end() { return fields_.end(); }
        [[nodiscard]] auto begin() const { return fields_.cbegin(); }
        [[nodiscard]] auto end() const { return fields_.cend(); }

    private:
        std::vector<std::unique_ptr<FieldBase>> fields_;
    };
}
