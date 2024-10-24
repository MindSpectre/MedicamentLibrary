#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>

#include "db_conditions.hpp"
#include "db_field.hpp"
#include "db_record.hpp"

namespace drug_lib::common::database::interfaces
{
    using namespace drug_lib::common::database;
    template <typename T>
    concept RecordContainer = std::is_same_v<std::remove_cvref_t<T>, std::vector<Record>>;
    template <typename T>
    concept FieldBaseVector = std::is_same_v<std::remove_cvref_t<T>, std::vector<std::unique_ptr<FieldBase>>>;

    class DbInterface
    {
    public:
        virtual ~DbInterface() = default;

        // Transaction Methods
        virtual void start_transaction() = 0;
        virtual void commit_transaction() = 0;
        virtual void rollback_transaction() = 0;

        virtual void drop_connect() = 0;
        // Table Management
        virtual void create_table(std::string_view table_name, const Record& field_list) = 0;
        virtual void remove_table(std::string_view table_name) = 0;
        [[nodiscard]] virtual bool check_table(std::string_view table_name) = 0;

        virtual void make_unique_constraint(std::string_view table_name,
                                            std::vector<std::shared_ptr<FieldBase>> conflict_fields) = 0;
        virtual void setup_full_text_search(
            std::string_view table_name,
            std::vector<std::shared_ptr<FieldBase>> fields) = 0;
        // Data Manipulation using Perfect Forwarding
        template <RecordContainer Rows>
        void insert(std::string_view table_name, Rows&& rows)
        {
            insert_implementation(table_name, std::forward<Rows>(rows));
        }

        // Upsert Data using Perfect Forwarding
        template <RecordContainer Rows>
        void upsert(std::string_view table_name,
                    Rows&& rows,
                    const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
        {
            upsert_implementation(table_name, std::forward<Rows>(rows), replace_fields);
        }

        // Data Retrieval
        [[nodiscard]] virtual std::vector<Record> select(
            std::string_view table_name) const = 0;
        // Data Retrieval
        [[nodiscard]] virtual std::vector<Record> select(
            std::string_view table_name,
            const FieldConditions& conditions) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<ViewRecord>> view(
            std::string_view table_name,
            const FieldConditions& conditions) const = 0;
        [[nodiscard]] virtual std::vector<std::unique_ptr<ViewRecord>> view(std::string_view table_name) const = 0;
        // Remove Data
        virtual void remove(
            std::string_view table_name,
            const FieldConditions& conditions) = 0;
        virtual void truncate_table(std::string_view table_name) = 0;
        [[nodiscard]] virtual uint32_t count(std::string_view table_name,
                                             const FieldConditions& conditions) const = 0;
        [[nodiscard]] virtual uint32_t count(std::string_view table_name) const = 0;
        // Full-Text Search Methods
        [[nodiscard]] virtual std::vector<Record> get_data_fts(
            std::string_view table_name,
            const std::string& fts_query_params) const = 0;

    protected:
        // Implementation Methods for Data Manipulation
        virtual void insert_implementation(std::string_view table_name, const std::vector<Record>& rows) = 0;
        virtual void insert_implementation(std::string_view table_name, std::vector<Record>&& rows) = 0;

        virtual void upsert_implementation(std::string_view table_name,
                                           const std::vector<Record>& rows,
                                           const std::vector<std::shared_ptr<FieldBase>>& replace_fields) = 0;

        virtual void upsert_implementation(std::string_view table_name,
                                           std::vector<Record>&& rows,
                                           const std::vector<std::shared_ptr<FieldBase>>& replace_fields) = 0;
    };
}
