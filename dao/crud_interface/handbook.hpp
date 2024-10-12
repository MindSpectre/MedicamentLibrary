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
        std::vector<std::shared_ptr<common::database::FieldBase>> fts_fields_;

        void setup_constraints() const
        {
            if (!table_name_.empty())
            {
                connect_->make_unique_constraint(table_name_,
                                                 {std::make_shared<common::database::Field<int32_t>>("id", 0)});
            }
            else
            {
                throw std::runtime_error("Table name must be set before constraints are applied.");
            }
        }

        [[nodiscard]] std::vector<RecordType> get(const common::database::FieldConditions& conditions) const
        {
            const auto result = connect_->select(table_name_, conditions);
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
            connect_->remove(table_name_, conditions);
        }

        void upsert(const std::vector<RecordType>& records,
                    const std::vector<std::shared_ptr<common::database::FieldBase>>& replace_fields) const
        {
            std::vector<common::database::Record> db_records;
            db_records.reserve(records.size());
            for (const auto& record : records)
            {
                db_records.push_back(record.to_record());
            }
            connect_->upsert(table_name_, db_records, replace_fields);
        }

    public:
        virtual ~HandbookBase() = default;

        void insert(const RecordType& record)
        {
            connect_->insert(table_name_, {record.to_record()});
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
            connect_->insert(table_name_, db_records);
        }

        virtual void update_all_fields(const RecordType& record) = 0;

        virtual void update_all_fields(const std::vector<RecordType>& records) = 0;

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
