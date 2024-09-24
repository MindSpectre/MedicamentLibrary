#pragma once

#include "db_field.hpp"
#include "db_conditions.hpp"
#include <vector>

namespace drug_lib::dao
{
    template <typename RecordType>
    class HandbookBase
    {
    public:
        virtual ~HandbookBase() = default;
        virtual void insert(const RecordType &record) = 0;
        virtual void insert(const std::vector<RecordType> &record) = 0;
        virtual void update(const RecordType &record) = 0;
        virtual void update(const std::vector<RecordType> &record) = 0;
        virtual void remove(const drug_lib::common::database::FieldConditions &conditions) = 0;
        virtual std::vector<RecordType> & get(const drug_lib::common::database::FieldConditions &conditions) = 0;
    };

}
