#pragma once

#include "db_record.hpp"

namespace drug_lib::data::objects
{
    class ObjectBase
    {
    public:
        virtual ~ObjectBase() = default;
        [[nodiscard]] virtual common::database::Record to_record() const = 0;
        virtual void from_record(const common::database::Record&) = 0;
    };
}
