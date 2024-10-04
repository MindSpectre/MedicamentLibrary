// db_record.hpp

#pragma once

#include "db_field.hpp"
#include <memory>
#include <string>
#include <boost/container/flat_map.hpp>

namespace drug_lib::common::database
{
    class Record final
    {
    public:
        ~Record() = default;

        void add_field(std::shared_ptr<FieldBase> field)
        {
            fields_[field->get_name()] = std::move(field);
        }

        [[nodiscard]] boost::container::flat_map<std::string, std::shared_ptr<FieldBase>> fields() const
        {
            return fields_;
        }

        // Access field by name
        std::shared_ptr<FieldBase>& operator[](const std::string& name)
        {
            return fields_[name];
        }

        [[nodiscard]] const std::shared_ptr<FieldBase>& at(const std::string& name) const
        {
            return fields_.at(name);
        }

        [[nodiscard]] std::shared_ptr<FieldBase> pull(const std::string& name)
        {
            return std::move(fields_.at(name));
        }

        // Implement begin() and end() methods
        auto begin() { return fields_.begin(); }
        auto end() { return fields_.end(); }
        [[nodiscard]] auto begin() const { return fields_.cbegin(); }
        [[nodiscard]] auto end() const { return fields_.cend(); }

    private:
        boost::container::flat_map<std::string, std::shared_ptr<FieldBase>> fields_;
    };
}
