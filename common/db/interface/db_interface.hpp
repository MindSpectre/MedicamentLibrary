#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string_view>
#include <vector>
#include <type_traits>
#include "db_conditions.hpp"
#include "db_field.hpp"
#include "db_record.hpp"

namespace drug_lib::common::database::interfaces
{
    using namespace drug_lib::common::database;
    template <typename T>
    concept RecordContainer = std::is_same_v<std::remove_cvref_t<T>, std::vector<Record>>;

    class DbInterface
    {
    public:
        virtual ~DbInterface() = default;

        // Transaction Methods
        virtual void start_transaction() = 0;
        virtual void commit_transaction() = 0;
        virtual void rollback_transaction() = 0;

        // Table Management
        virtual void create_table(std::string_view table_name, const Record& field_list) = 0;
        virtual void remove_table(std::string_view table_name) = 0;
        [[nodiscard]] virtual bool check_table(std::string_view table_name) const = 0;

        virtual void make_unique_constraint(std::string_view table_name,
                                            std::vector<std::shared_ptr<FieldBase>>&& conflict_fields) = 0;

        // Data Manipulation using Perfect Forwarding
        template <RecordContainer Rows>
        void add_data(std::string_view table_name, Rows&& rows)
        {
            add_data_impl(table_name, std::forward<Rows>(rows));
        }

        // Upsert Data using Perfect Forwarding
        template <RecordContainer Rows>
        void upsert_data(std::string_view table_name,
                         Rows&& rows,

                         const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
        {
            upsert_data_impl(table_name, std::forward<Rows>(rows), replace_fields);
        }

        // Data Retrieval
        [[nodiscard]] virtual std::vector<Record> get_data(
            std::string_view table_name,
            const FieldConditions& conditions) const = 0;

        // Remove Data
        virtual void remove_data(
            std::string_view table_name,
            const FieldConditions& conditions) = 0;

        // Get Record Count
        [[nodiscard]] virtual int get_count(std::string_view table_name,
                                            const std::shared_ptr<FieldBase>& field,
                                            std::chrono::duration<double>& query_exec_time) const = 0;

        // Full-Text Search Methods
        [[nodiscard]] virtual std::vector<Record> get_data_fts(
            std::string_view table_name,
            std::string_view fts_query_params,
            std::chrono::duration<double>& query_exec_time) const = 0;

        virtual bool get_data_fts_batched(
            std::string_view table_name,
            std::string_view fts_query_params,
            std::chrono::duration<double>& query_exec_time,
            const std::function<void(const std::vector<Record>&)>& on_result) const = 0;

    protected:
        // Implementation Methods for Data Manipulation
        virtual void add_data_impl(std::string_view table_name, const std::vector<Record>& rows) = 0;
        virtual void add_data_impl(std::string_view table_name, std::vector<Record>&& rows) = 0;

        virtual void upsert_data_impl(std::string_view table_name,
                                      const std::vector<Record>& rows,

                                      const std::vector<std::shared_ptr<FieldBase>>& replace_fields) = 0;

        virtual void upsert_data_impl(std::string_view table_name,
                                      std::vector<Record>&& rows,

                                      const std::vector<std::shared_ptr<FieldBase>>& replace_fields) = 0;
    };
}
