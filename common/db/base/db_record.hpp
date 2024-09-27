// db_record.hpp

#pragma once

#include "db_field.hpp"

#include <unordered_map>
#include <memory>
#include <string>

namespace drug_lib::common::database
{
    class Record final
    {
    public:
        virtual ~Record() = default;

        void add_field(std::shared_ptr<FieldBase> field)
        {
            fields_[field->get_name()] = std::move(field);
        }

        [[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<FieldBase>>& fields() const
        {
            return fields_;
        }

        // Access field by name
        std::shared_ptr<FieldBase>& operator[](const std::string& name)
        {
            return fields_[name];
        }

        std::shared_ptr<FieldBase> at(const std::string& name) const
        {
            return fields_.at(name);
        }

        // Implement begin() and end() methods
        auto begin() { return fields_.begin(); }
        auto end() { return fields_.end(); }
        auto begin() const { return fields_.cbegin(); }
        auto end() const { return fields_.cend(); }

    private:
        std::unordered_map<std::string, std::shared_ptr<FieldBase>> fields_;
    };
}
