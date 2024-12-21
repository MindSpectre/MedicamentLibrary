#pragma once
#include <utility>
#include <vector>

#include "auth_object.hpp"
#include "db_conditions.hpp"
#include "db_interface.hpp"
#include "db_interface_pool.hpp"
#include "error_codes.hpp"
#include "exceptions.hpp"

namespace drug_lib::dao
{
    class AuthDataHolder final
    {
    public:
        void establish_from_pool(common::database::creational::DbInterfacePool& pool)
        {
            connect_ = pool.acquire_db_interface();
            used_pool = pool;
            this->setup();
        }

        ~AuthDataHolder() = default;

        AuthDataHolder() = default;

        void insert(const data::objects::AuthObject& record) const
        {
            std::vector<common::database::Record> db_record;
            db_record.push_back(record.to_record());
            connect_->insert(table_name_, std::move(db_record));
        }

        // Insert multiple records
        void insert(const std::vector<data::objects::AuthObject>& records) const
        {
            std::vector<common::database::Record> db_records;
            db_records.reserve(records.size());
            for (const auto& record : records)
            {
                db_records.push_back(record.to_record());
            }
            connect_->insert(table_name_, std::move(db_records));
        }

        void force_insert(const data::objects::AuthObject& record) const
        {
            std::vector<common::database::Record> db_records;
            db_records.push_back(record.to_record());
            connect_->upsert(table_name_, std::move(db_records), value_fields_);
        }

        void force_insert(const std::vector<data::objects::AuthObject>& records) const
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
                std::make_unique<common::database::Field<int32_t>>(
                    data::objects::auth_object::field_name::user_id,
                    0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
            );
            connect_->remove(table_name_, removed_conditions);
        }

        void remove_by_login(const std::string& login) const
        {
            common::database::Conditions removed_conditions;
            removed_conditions.add_field_condition(
                std::make_unique<common::database::Field<std::string>>(
                    data::objects::auth_object::field_name::login, ""),
                "=",
                std::make_unique<common::database::Field<std::string>>("", login)
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

        [[nodiscard]] data::objects::AuthObject get_by_user_id(const int32_t id) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<int32_t>>(
                    data::objects::auth_object::field_name::user_id,
                    0),
                "=",
                std::make_unique<common::database::Field<int32_t>>("", id)
            );
            const auto res = connect_->select(table_name_, select_conditions);
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
            data::objects::AuthObject record;
            record.from_record(res.front());
            return record;
        }

        [[nodiscard]] data::objects::AuthObject get_by_login(const std::string& login) const
        {
            common::database::Conditions select_conditions;
            select_conditions.add_field_condition(
                std::make_unique<common::database::Field<std::string>>(
                    data::objects::auth_object::field_name::login, ""),
                "=",
                std::make_unique<common::database::Field<std::string>>("", login)
            );
            const auto res = connect_->select(table_name_, select_conditions);
            if (res.size() > 1)
            {
                throw common::database::exceptions::InvalidIdentifierException(
                    "Not unique record", common::database::errors::db_error_code::DUPLICATE_RECORD);
            }
            if (res.empty())
            {
                throw common::database::exceptions::InvalidIdentifierException(
                    "Record not found", common::database::errors::db_error_code::RECORD_NOT_FOUND);
            }
            data::objects::AuthObject record;
            record.from_record(res.front());
            return record;
        }

        void set_connection(std::shared_ptr<common::database::interfaces::DbInterface> connect)
        {
            connect_ = std::move(connect);
            this->setup();
        }

        void drop_connection()
        {
            if (used_pool)
            {
                used_pool->get().release_db_interface(std::move(connect_));
            }
            else connect_->drop_connect();
        }

        [[nodiscard]] std::shared_ptr<common::database::interfaces::DbInterface> get_connection() const
        {
            return connect_;
        }

    private:
        std::shared_ptr<common::database::interfaces::DbInterface> connect_;
        std::optional<std::reference_wrapper<common::database::creational::DbInterfacePool>> used_pool;
        std::string table_name_;

        std::vector<std::shared_ptr<common::database::FieldBase>> key_fields_;
        std::vector<std::shared_ptr<common::database::FieldBase>> value_fields_;

        void setup() &
        {
            if (!connect_)
            {
                throw std::runtime_error("Cannot connect to database interface.");
            }
            //creating fields
            const auto id_field = common::database::make_field_shared_by_default<int32_t>(
                data::objects::auth_object::field_name::user_id);
            const auto login_field = common::database::make_field_shared_by_default<std::string>(
                data::objects::auth_object::field_name::login);
            const auto password_field = common::database::make_field_shared_by_default<std::string>(
                data::objects::auth_object::field_name::password);

            value_fields_.push_back(password_field);
            value_fields_.push_back(id_field);

            key_fields_.push_back(login_field);

            if (!table_name_.empty())
            {
                if (connect_->check_table(table_name_))
                {
                    connect_->set_conflict_fields(table_name_, key_fields_);
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
            }
            else
            {
                throw std::runtime_error("Table name must be set before constraints are applied.");
            }
        }
    };
}
