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
            { T::fields::id };
            { T::fields::name };
        };

    template <RecordTypeConcept RecordType>
    class HandbookBase
    {
    protected:
        std::shared_ptr<common::database::interfaces::DbInterface> connect_;
        std::string_view table_name_;
        std::vector<std::shared_ptr<common::database::FieldBase>> fts_fields_;
        std::vector<std::shared_ptr<common::database::FieldBase>> replaceable_fields_;
        std::vector<std::shared_ptr<common::database::FieldBase>> key_fields_;
        std::vector<std::shared_ptr<common::database::FieldBase>> value_fields_;

        virtual void setup(std::shared_ptr<common::database::interfaces::DbInterface> client) &
        {
            connect_ = std::move(client);
            if (!table_name_.empty())
            {
                if (connect_->check_table(table_name_))
                    return;
                common::database::Record record;
                for (const auto& field : key_fields_)
                {
                    record.push_back(field->clone());
                }
                for (const auto& field : value_fields_)
                {
                    record.push_back(field->clone());
                }
                connect_->create_table(table_name_, record);
                connect_->make_unique_constraint(table_name_, key_fields_);
                connect_->setup_fts_index(table_name_, fts_fields_);
            }
            else
            {
                throw std::runtime_error("Table name must be set before constraints are applied.");
            }
        }

    public:
        virtual ~HandbookBase() = default;

        void insert(const RecordType& record)
        {
            std::vector db_record = {record.to_record()};
            connect_->insert(table_name_, std::move(db_record));
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
            connect_->insert(table_name_, std::move(db_records));
        }

        void force_insert(const RecordType& record)
        {
            std::vector<common::database::Record> db_records;
            db_records.push_back(record.to_record());
            connect_->upsert(table_name_, std::move(db_records), replaceable_fields_);
        }

        void force_insert(const std::vector<RecordType>& records)
        {
            std::vector<common::database::Record> db_records;
            db_records.reserve(records.size());
            for (const auto& record : records)
            {
                db_records.push_back(record.to_record());
            }
            connect_->upsert(table_name_, std::move(db_records), replaceable_fields_);
        }

        void remove_by_id(int32_t id) const
        {
            common::database::Conditions removed_conditions;
            removed_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>("id", 0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
            );
            connect_->remove(table_name_, removed_conditions);
        }

        void remove_by_name(const std::string& name) const
        {
            common::database::Conditions removed_conditions;
            removed_conditions.add_field_condition(
                std::make_unique<common::database::Field<std::string>>("name", ""),
                "=",
                std::make_unique<common::database::Field<std::string>>("", name)
            );
            connect_->remove(table_name_, removed_conditions);
        }

        std::vector<RecordType> get_by_id(int32_t id) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(RecordType::fields::id, 0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
            );
            auto res = connect_->view(table_name_, select_conditions);
            std::vector<RecordType> records;
            records.reserve(res.size());
            for (const auto& record : res)
            {
                RecordType tmp;
                tmp.from_record(record);
                records.push_back(std::move(tmp));
            }
            return records;
        }

        std::vector<RecordType> get_by_name(const std::string& name) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<std::string>>(RecordType::fields::name, ""),
                "=",
                std::make_unique<common::database::Field<std::string>>("", name)
            );
            auto res = connect_->view(table_name_, select_conditions);
            std::vector<RecordType> records;
            records.reserve(res.size());
            for (const auto& record : res)
            {
                RecordType tmp;
                tmp.from_record(record);
                records.push_back(std::move(tmp));
            }
            return records;
        }

        std::vector<RecordType> get_by_id_paged(int32_t id, const std::size_t page_number,
                                                const uint16_t page_limit) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(RecordType::fields::id, 0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
            );
            select_conditions.set_page_condition(
                common::database::PageCondition(page_limit).set_page_number(page_number));
            auto res = connect_->select(table_name_, select_conditions);
            std::vector<RecordType> records;
            records.reserve(res.size());
            for (const auto& record : res)
            {
                RecordType tmp;
                tmp.from_record(record);
                records.push_back(std::move(tmp));
            }
            return records;
        }

        std::vector<RecordType> get_by_name_paged(const std::string& name, const std::size_t page_number,
                                                  const uint16_t page_limit) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(RecordType::fields::name, 0),
                "=",
                std::make_unique<common::database::Field<std::string>>("", name)
            );
            select_conditions.set_page_condition(
                common::database::PageCondition(page_limit).set_page_number(page_number));
            auto res = connect_->select(table_name_, select_conditions);
            std::vector<RecordType> records;
            records.reserve(res.size());
            for (const auto& record : res)
            {
                RecordType tmp;
                tmp.from_record(record);
                records.push_back(std::move(tmp));
            }
            return records;
        }

        std::vector<RecordType> search(const std::string& pattern) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_pattern_condition(pattern);
            auto res = connect_->view(table_name_, select_conditions);
            std::vector<RecordType> records;
            records.reserve(res.size());
            for (const auto& record : res)
            {
                RecordType tmp;
                tmp.from_record(record);
                records.push_back(std::move(tmp));
            }
            return records;
        }

        virtual void tear_down() = 0;

        std::vector<RecordType> select()
        {
            auto res = connect_->select(table_name_);
            std::vector<RecordType> records;
            records.reserve(res.size());
            for (const auto& record : res)
            {
                RecordType tmp;
                tmp.from_record(record);
                records.push_back(std::move(tmp));
            }
            return records;
        }

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
