#pragma once

#include "db_field.hpp"

#include <string>
#include <memory>
#include <vector>


namespace drug_lib::data
{
    /// @brief class for describe EQUAL conditions for db fields 
    /// @warning ONLY SUPPORT EQUAL 
    class FieldConditions {
    public:
        virtual ~FieldConditions() = default;
        /// @brief add additional condition to collection
        /// @param field condition where FieldBase.name - field name, FieldBase.value == condition
        /// @attention moveable
        void add_field(std::shared_ptr<FieldBase> field) {
            fields_.push_back(std::move(field));
        }
        /// @return acceptable collection of conditions
        const std::vector<std::shared_ptr<FieldBase>>& fields() const {
            return fields_;
        }

    private:
        std::vector<std::shared_ptr<FieldBase>> fields_;
    };

}