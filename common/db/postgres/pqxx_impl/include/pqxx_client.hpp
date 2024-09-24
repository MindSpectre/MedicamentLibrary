// pqxx_client.hpp
#pragma once

#include <pqxx/pqxx>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <mutex>
#include <regex>
#include "db_interface.hpp"
#include "exceptions.hpp"

namespace drug_lib::common::database
{
    class PqxxClient final : public interfaces::DbInterface
    {
    public:
        PqxxClient(std::string_view host,
                   int port,
                   std::string_view db_name,
                   std::string_view login,
                   std::string_view password);

        ~PqxxClient() override = default;

        // Transaction Methods
        void start_transaction() override;
        void commit_transaction() override;
        void rollback_transaction() override;

        // Table Management
        void create_table(std::string_view table_name, const Record& field_list) override;
        void remove_table(std::string_view table_name) override;
        [[nodiscard]] bool check_table(std::string_view table_name) const override;

        // Data Manipulation
        template <typename Rows>
        void add_data(std::string_view table_name, Rows&& rows)
        {
            add_data_impl(table_name, std::forward<Rows>(rows));
        }

        template <typename Rows>
        void upsert_data(std::string_view table_name,
                         Rows&& rows,
                         const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                         const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
        {
            upsert_data_impl(table_name, std::forward<Rows>(rows), conflict_fields, replace_fields);
        }

        // Data Retrieval
        [[nodiscard]] std::vector<Record> get_data(
            std::string_view table_name,
            const FieldConditions& conditions) const override;

        // Remove Data
        void remove_data(
            std::string_view table_name,
            const FieldConditions& conditions) override;

        // Get Record Count
        [[nodiscard]] int get_count(std::string_view table_name,
                                    const std::shared_ptr<FieldBase>& field,
                                    std::chrono::duration<double>& query_exec_time) const override;

        // Full-Text Search Methods
        [[nodiscard]] std::vector<Record> get_data_fts(
            std::string_view table_name,
            std::string_view fts_query_params,
            std::chrono::duration<double>& query_exec_time) const override;

        bool get_data_fts_batched(
            std::string_view table_name,
            std::string_view fts_query_params,
            std::chrono::duration<double>& query_exec_time,
            const std::function<void(const std::vector<Record>&)>& on_result) const override;

    protected:
        // Implementation Methods for Data Manipulation
        void add_data_impl(std::string_view table_name, const std::vector<Record>& rows) override;
        void add_data_impl(std::string_view table_name, std::vector<Record>&& rows) override;

        void upsert_data_impl(std::string_view table_name,
                              const std::vector<Record>& rows,
                              const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                              const std::vector<std::shared_ptr<FieldBase>>& replace_fields) override;

        void upsert_data_impl(std::string_view table_name,
                              std::vector<Record>&& rows,
                              const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                              const std::vector<std::shared_ptr<FieldBase>>& replace_fields) override;

    private:
        std::shared_ptr<pqxx::connection> conn_;
        mutable std::mutex conn_mutex_;
        bool in_transaction_;

        // Utility Methods
        [[nodiscard]] static bool is_valid_identifier(std::string_view identifier);
        void execute_query(const std::string& query_string, const pqxx::params& params = {}) const;
        std::pair<std::string, pqxx::params> construct_insert_query(
            std::string_view table_name,
            const std::vector<Record>& rows) const;

        std::pair<std::string, pqxx::params> construct_insert_query(
            std::string_view table_name,
            std::vector<Record>&& rows) const;

        std::string escape_identifier(std::string_view identifier) const;
    };
}
