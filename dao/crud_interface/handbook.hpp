#pragma once

#include <utility>
#include <vector>

#include "common_obj.hpp"
#include "db_conditions.hpp"
#include "db_interface.hpp"

namespace drug_lib::dao
{
    namespace handbook_tables_name
    {
        constexpr std::string_view Medicaments = "drugs";
        constexpr std::string_view Organizations = "organizations";
        constexpr std::string_view Patients = "patients";
        constexpr std::string_view ConfigOperations = "config_operations";
    }

    template <typename T>
    concept RecordTypeConcept = std::derived_from<T, data::objects::ObjectBase> &&
        requires(T a, common::database::Record record)
        {
            { a.to_record() } -> std::same_as<common::database::Record>;
            { a.from_record(record) } -> std::same_as<void>;
        };

    template <RecordTypeConcept RecordType>
    class HandbookBase
    {
    protected:
        std::shared_ptr<common::database::interfaces::DbInterface> connect_;
        std::string_view table_name_;

        std::vector<RecordType> get(const common::database::FieldConditions& conditions) const
        {
            const auto result = connect_->get_data(table_name_, conditions);
            std::vector<RecordType> records;
            records.reserve(result.size());
            for (const auto& db_record : result)
            {
                RecordType record;
                record.from_record(db_record);
                records.emplace_back(std::move(record));
            }
            return records;
        }

        void remove(const common::database::FieldConditions& conditions) const
        {
            connect_->remove_data(table_name_, conditions);
        }

    public:
        virtual ~HandbookBase() = default;

        void insert(const RecordType& record)
        {
            connect_->add_data(table_name_, record.to_record());
        }

        // Insert multiple records
        void insert(const std::vector<RecordType>& records)
        {
            std::vector<common::database::Record> db_records;
            db_records.reserve(records.size());
            for (const auto& record : records)
            {
                db_records.push_back(record.to_record());
            }
            connect_->add_data(table_name_, db_records);
        }

        void update(const RecordType& record)
        {
        }

        void update(const std::vector<RecordType>& record)
        {
        }

        virtual void remove_all() = 0;
        virtual std::vector<RecordType> get_all() = 0;

        void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect)
        {
            connect_ = std::move(connect);
        }

        void drop_connection() const
        {
            connect_->drop_connect();
        }

        [[nodiscard]] std::shared_ptr<common::database::interfaces::DbInterface> get_connection() const
        {
            return connect_;
        }
    };
}
