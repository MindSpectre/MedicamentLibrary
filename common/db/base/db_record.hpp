#pragma once

#include "db_field.hpp"

#include <vector>
#include <memory>

namespace drug_lib::common::db
{
    class Record {
    public:
        virtual ~Record() = default;

        void add_field(std::shared_ptr<FieldBase> field) {
            fields_.push_back(std::move(field));
        }

        const std::vector<std::shared_ptr<FieldBase>>& fields() const {
            return fields_;
        }

    private:
        std::vector<std::shared_ptr<FieldBase>> fields_;
    };
}

