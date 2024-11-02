#include "medicaments_handbook.hpp"
#include <pqxx/except>

namespace drug_lib::dao
{
    void MedicamentsHandbook::force_insert(const objects::Medicament& record)
    {
        std::vector<common::database::Record> db_records;
        db_records.push_back(record.to_record());
        connect_->upsert(table_name_, std::move(db_records), replaceable_fields_);
    }

    void MedicamentsHandbook::force_insert(const std::vector<objects::Medicament>& records)
    {
        std::vector<common::database::Record> db_records;
        db_records.reserve(records.size());
        for (const auto& record : records)
        {
            db_records.push_back(record.to_record());
        }
        connect_->upsert(table_name_, std::move(db_records), replaceable_fields_);
    }

    void MedicamentsHandbook::remove_by_id(int32_t id)
    {
        common::database::Conditions removed_conditions;
        removed_conditions.add_field_condition(common::database::FieldCondition(
            std::make_unique<common::database::Field<int32_t>>("id", 0),
            "=",
            std::make_unique<common::database::Field<int32_t>>("", id)
        ));
        connect_->remove(table_name_, removed_conditions);
    }

    void MedicamentsHandbook::remove_by_name(const std::string& name)
    {
        common::database::Conditions removed_conditions;
        removed_conditions.add_field_condition(common::database::FieldCondition(
            std::make_unique<common::database::Field<int32_t>>("name", 0),
            "=",
            std::make_unique<common::database::Field<std::string>>("", name)
        ));
        connect_->remove(table_name_, removed_conditions);
    }

    std::vector<objects::Medicament> MedicamentsHandbook::get_by_id(int32_t id)
    {
        common::database::Conditions select_conditions;
        select_conditions.add_field_condition(common::database::FieldCondition(
            std::make_unique<common::database::Field<int32_t>>(objects::Medicament::fields::id, 0),
            "=",
            std::make_unique<common::database::Field<int32_t>>("", id)
        ));
        auto res = connect_->view(table_name_, select_conditions);
        std::vector<objects::Medicament> records;
        records.reserve(res.size());
        for (const auto& record : res)
        {
            objects::Medicament tmp;
            tmp.from_record(record);
            records.push_back(std::move(tmp));
        }
        return records;
    }

    std::vector<objects::Medicament> MedicamentsHandbook::get_by_name(const std::string& name)
    {
        common::database::Conditions select_conditions;
        select_conditions.add_field_condition(common::database::FieldCondition(
            std::make_unique<common::database::Field<int32_t>>(objects::Medicament::fields::name, 0),
            "=",
            std::make_unique<common::database::Field<std::string>>("", name)
        ));
        auto res = connect_->view(table_name_, select_conditions);
        std::vector<objects::Medicament> records;
        records.reserve(res.size());
        for (const auto& record : res)
        {
            objects::Medicament tmp;
            tmp.from_record(record);
            records.push_back(std::move(tmp));
        }
        return records;
    }

    std::vector<objects::Medicament> MedicamentsHandbook::search(const std::string& pattern)
    {
        common::database::Conditions select_conditions;
        select_conditions.add_pattern_condition(common::database::PatternCondition(pattern));
        auto res = connect_->view(table_name_, select_conditions);
        std::vector<objects::Medicament> records;
        records.reserve(res.size());
        for (const auto& record : res)
        {
            objects::Medicament tmp;
            tmp.from_record(record);
            records.push_back(std::move(tmp));
        }
        return records;
    }

    void MedicamentsHandbook::tear_down()
    {
    }

    std::vector<objects::Medicament> MedicamentsHandbook::select()
    {
        auto res = connect_->select(table_name_);
        std::vector<objects::Medicament> records;
        records.reserve(res.size());
        for (const auto& record : res)
        {
            objects::Medicament tmp;
            tmp.from_record(record);
            records.push_back(std::move(tmp));
        }
        return records;
    }

    void MedicamentsHandbook::setup(std::shared_ptr<common::database::interfaces::DbInterface> client) &
    {
        table_name_ = handbook_tables_name::Medicaments;
        // Create a test table
        // Record fields;
        // fields.push_back(std::make_unique<Field<int>>("id", 0));
        // fields.push_back(std::make_unique<Field<std::string>>("name", ""));
        // fields.push_back(std::make_unique<common::database::Field<std::string>>("description", ""));
        // db_client->create_table(test_table, fields);
        // Set up full-text search on 'name' and 'description' fields
        //creating fields
        auto id_f = common::database::Field<int32_t>(objects::Medicament::fields::id, 0);
        auto name_f = common::database::Field<std::string>(objects::Medicament::fields::name, "");
        auto prop_f = common::database::Field<Json::Value>(objects::Medicament::fields::properties, Json::Value());
        fts_fields_.push_back(std::make_shared<common::database::Field<std::string>>(name_f));
        fts_fields_.push_back(std::make_shared<common::database::Field<Json::Value>>(prop_f));

        value_fields_.push_back(std::make_shared<common::database::Field<std::string>>(name_f));
        value_fields_.push_back(std::make_shared<common::database::Field<Json::Value>>(prop_f));

        key_fields_.push_back(std::make_shared<common::database::Field<int32_t>>(id_f));

        common::database::Record record;
        record.push_back(std::make_unique<common::database::Field<int32_t>>(id_f));
        record.push_back(std::make_unique<common::database::Field<std::string>>(name_f));
        record.push_back(std::make_unique<common::database::Field<Json::Value>>(prop_f));

        HandbookBase::setup(std::move(client));
    }
}
