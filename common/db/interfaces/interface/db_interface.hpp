#pragma once

#include "db_field.hpp"
#include "db_record.hpp"

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace drug_lib::common::db::interfaces
{
    using namespace drug_lib::data;
    class DBInterface
    {
    public:
        virtual ~DBInterface() = default;

        // Методы транзакций
        virtual void start_transaction() = 0;
        virtual void commit_transaction() = 0;
        virtual void rollback_transaction() = 0;

        // Управление таблицами
        virtual void create_table(const std::string& table_name, const Record& field_list) = 0;
        virtual void remove_table(const std::string& table_name) = 0;
        virtual bool has_table(const std::string& table_name) const = 0;

        // Манипуляция данными
        virtual void add_data(const std::string& table_name, const std::vector<Record>& rows) = 0;
        virtual void add_data(const std::string& table_name, std::vector<Record>&& rows) = 0;

        // Upsert данные с использованием полей конфликта и замены
        virtual void upsert_data(const std::string& table_name,
                                 const std::vector<Record>& rows,
                                 const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                                 const std::vector<std::shared_ptr<FieldBase>>& replace_fields) = 0;

        virtual void upsert_data(const std::string& table_name,
                                 std::vector<Record>&& rows,
                                 const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                                 const std::vector<std::shared_ptr<FieldBase>>& replace_fields) = 0;

        // Получение данных
        virtual std::vector<Record> get_data(
            const std::string& table_name,
            const std::vector<std::shared_ptr<FieldBase>>& conditions) = 0;

        virtual std::vector<Record> get_data(
            const std::string& table_name,
            std::vector<std::shared_ptr<FieldBase>>&& conditions) = 0;

        // Удаление данных
        virtual void remove_data(
            const std::string& table_name,
            const std::vector<std::shared_ptr<FieldBase>>& conditions) = 0;

        virtual void remove_data(
            const std::string& table_name,
            std::vector<std::shared_ptr<FieldBase>>&& conditions) = 0;

        // Получение количества записей
        virtual int get_count(const std::string& table_name, const std::shared_ptr<FieldBase>& field, double& query_exec_time) = 0;

        // Методы полнотекстового поиска
        virtual std::vector<Record> get_data_fts(
            const std::string& table_name,
            const std::string& fts_query_params,
            double& query_exec_time) = 0;

        virtual bool get_data_fts_batched(
            const std::string& table_name,
            const std::string& fts_query_params,
            double& query_exec_time,
            std::function<void(const std::vector<Record>&)> on_result) = 0;

        // Вспомогательные шаблонные методы для удобства
        template <typename... Fields>
        void remove_data(const std::string& table_name, Fields... fields)
        {
            std::vector<std::shared_ptr<FieldBase>> conditions = {std::make_shared<Fields>(fields)...};
            remove_data(table_name, std::move(conditions));
        }

        template <typename... Fields>
        std::vector<Record> get_data(const std::string& table_name, Fields... fields)
        {
            std::vector<std::shared_ptr<FieldBase>> conditions = {std::make_shared<Fields>(fields)...};
            return get_data(table_name, std::move(conditions));
        }

        // Вспомогательные шаблонные методы для upsert_data
        template <typename... Fields>
        void upsert_data(const std::string& table_name,
                         const std::vector<Record>& rows,
                         std::vector<std::shared_ptr<FieldBase>> conflict_fields,
                         std::vector<std::shared_ptr<FieldBase>> replace_fields)
        {
            upsert_data(table_name, rows, conflict_fields, replace_fields);
        }

        template <typename... Fields>
        void upsert_data(const std::string& table_name,
                         std::vector<Record>&& rows,
                         std::vector<std::shared_ptr<FieldBase>> conflict_fields,
                         std::vector<std::shared_ptr<FieldBase>> replace_fields)
        {
            upsert_data(table_name, std::move(rows), conflict_fields, replace_fields);
        }
    };
}
