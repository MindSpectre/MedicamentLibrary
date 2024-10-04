#pragma once

#include "db_field.hpp"
#include "db_conditions.hpp"
#include "db_interface.hpp"
#include <vector>

namespace drug_lib::dao
{
    namespace handbook_tables_name
    {
        constexpr std::string_view Medicaments = "drugs";
        constexpr std::string_view Organizations = "organizations";
        constexpr std::string_view Patients = "patients";
        constexpr std::string_view ConfigOperations = "config_operations";
    }


    template <typename RecordType>
    class HandbookBase
    {
    public:
        virtual ~HandbookBase() = default;
        virtual void insert(const RecordType& record) = 0;
        virtual void insert(const std::vector<RecordType>& record) = 0;
        virtual void update(const RecordType& record) = 0;
        virtual void update(const std::vector<RecordType>& record) = 0;
        virtual void remove(const common::database::FieldConditions& conditions) = 0;
        virtual std::vector<RecordType>& get(const common::database::FieldConditions& conditions) = 0;
        virtual void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect) = 0;
        virtual void drop_connection() = 0;
        virtual std::shared_ptr<common::database::interfaces::DbInterface> get_connection() = 0;
    };
}
