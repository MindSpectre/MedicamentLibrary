#pragma once

#include <utility>
#include <vector>

#include "common_object.hpp"
#include "db_conditions.hpp"
#include "db_interface.hpp"
#include "error_codes.hpp"
#include "exceptions.hpp"

namespace drug_lib::dao
{
    namespace handbook_tables_name
    {
        constexpr auto Medicaments = "medicaments";
        constexpr auto Organizations = "organizations";
        constexpr auto Patients = "patients";
        constexpr auto Diseases = "diseases";
        constexpr auto ConfigOperations = "config_operations";
    }

    template <typename T>
    concept RecordTypeConcept = std::derived_from<T, data::objects::ObjectBase> &&
        requires(T a, common::database::Record record)
        {
            { a.to_record() } -> std::same_as<common::database::Record>;
            { a.from_record(record) } -> std::same_as<void>;
            { T::field_name::id };
            { T::field_name::name };
        };

    template <RecordTypeConcept RecordType>
    class HandbookBase
    {
    protected:
        std::shared_ptr<common::database::interfaces::DbInterface> connect_;
        std::string_view table_name_;
        std::vector<std::shared_ptr<common::database::FieldBase>> fts_fields_;
        std::vector<std::shared_ptr<common::database::FieldBase>> key_fields_;
        std::vector<std::shared_ptr<common::database::FieldBase>> value_fields_;

        virtual void setup() &
        {
            if (!connect_)
            {
                throw std::runtime_error("Cannot connect to database interface.");
            }
            //creating fields
            const auto id_field = common::database::make_field_shared_by_default<int32_t>(
                data::objects::ObjectBase::common_fields_names::id);
            const auto name_field = common::database::make_field_shared_by_default<std::string>(
                data::objects::ObjectBase::common_fields_names::name);
            const auto properties_field = common::database::make_field_shared_by_default<Json::Value>(
                data::objects::ObjectBase::common_fields_names::properties);
            fts_fields_.push_back(name_field);
            fts_fields_.push_back(properties_field);

            value_fields_.push_back(name_field);
            value_fields_.push_back(properties_field);

            key_fields_.push_back(id_field);

            if (!table_name_.empty())
            {
                if (connect_->check_table(table_name_))
                {
                    connect_->set_conflict_fields(table_name_, key_fields_);
                    connect_->set_search_fields(table_name_, fts_fields_);
                    return;
                }
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
                connect_->setup_search_index(table_name_, fts_fields_);
            }
            else
            {
                throw std::runtime_error("Table name must be set before constraints are applied.");
            }
        }

        virtual void tear_down() = 0;

    public:
        virtual ~HandbookBase() = default;

        HandbookBase() = default;

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
            connect_->upsert(table_name_, std::move(db_records), value_fields_);
        }

        void force_insert(const std::vector<RecordType>& records)
        {
            std::vector<common::database::Record> db_records;
            db_records.reserve(records.size());
            for (const auto& record : records)
            {
                db_records.push_back(record.to_record());
            }
            connect_->upsert(table_name_, std::move(db_records), value_fields_);
        }

        void remove_by_id(const int32_t id) const
        {
            common::database::Conditions removed_conditions;
            removed_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(data::objects::ObjectBase::common_fields_names::id,
                                                                   0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
                );
            connect_->remove(table_name_, removed_conditions);
        }

        void remove_by_name(const std::string& name) const
        {
            common::database::Conditions removed_conditions;
            removed_conditions.add_field_condition(
                std::make_unique<common::database::Field<std::string>>(
                    data::objects::ObjectBase::common_fields_names::name, ""),
                "=",
                std::make_unique<common::database::Field<std::string>>("", name)
                );
            connect_->remove(table_name_, removed_conditions);
        }

        void remove_all() const
        {
            connect_->truncate_table(table_name_);
        }

        void delete_table() const
        {
            connect_->remove_table(table_name_);
        }

        RecordType get_by_id(const int32_t id) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(data::objects::ObjectBase::common_fields_names::id,
                                                                   0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
                );
            auto res = connect_->view(table_name_, select_conditions);
            if (res.size() > 1)
            {
                throw common::database::exceptions::InvalidIdentifierException(
                    "Not unique record", common::database::errors::db_error_code::DUPLICATE_RECORD);
            }
            if (res.empty())
            {
                throw common::database::exceptions::InvalidIdentifierException(
                    "Record not found", common::database::errors::db_error_code::DUPLICATE_RECORD);
            }
            RecordType record;
            record.from_record(res.front());
            return record;
        }

        std::vector<RecordType> get_by_name(const std::string& name) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<std::string>>(
                    data::objects::ObjectBase::common_fields_names::name, ""),
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


        std::vector<RecordType> get_by_name_paged(const std::string& name, const uint16_t page_limit,
                                                  const std::size_t page_number = 1) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(data::objects::ObjectBase::common_fields_names::name,
                                                                   0),
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

        std::vector<RecordType> search_paged(const std::string& pattern, const uint16_t page_limit,
                                             const std::size_t page_number = 1) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_pattern_condition(pattern);
            select_conditions.set_page_condition(
                common::database::PageCondition(page_limit).set_page_number(page_number));
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

        std::vector<RecordType> fuzzy_search_paged(const std::string& pattern, const uint16_t page_limit,
                                                   const std::size_t page_number = 1) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_similarity_condition(pattern);
            select_conditions.set_page_condition(
                common::database::PageCondition(page_limit).set_page_number(page_number));
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

        virtual void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect)
        {
            connect_ = std::move(connect);
            this->setup();
        }

        void drop_connection()
        {
            tear_down();
            connect_->drop_connect();
        }

        [[nodiscard]] std::shared_ptr<common::database::interfaces::DbInterface> get_connection() const
        {
            return connect_;
        }
    };
}
