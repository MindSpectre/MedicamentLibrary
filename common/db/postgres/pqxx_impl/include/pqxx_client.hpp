// pqxx_client.hpp
#pragma once

#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include <boost/container/flat_map.hpp>
#include <pqxx/pqxx>

#include "db_interface.hpp"
#include "exceptions.hpp"
#include "pqxx_connect_params.hpp"


namespace drug_lib::common::database
{
    class PqxxClient final : public interfaces::DbInterface
    {
    public:
        static void create_database(std::string_view host,
                                    uint32_t port,
                                    std::string_view db_name,
                                    std::string_view login,
                                    std::string_view password);
        static void create_database(const PqxxConnectParams& pr);
        void drop_connect() override;

        PqxxClient(std::string_view host,
                   uint32_t port,
                   std::string_view db_name,
                   std::string_view login,
                   std::string_view password);
        explicit PqxxClient(const PqxxConnectParams& pr);
        ~PqxxClient() override = default;

        // Transaction Methods
        void start_transaction() override;
        void commit_transaction() override;
        void rollback_transaction() override;

        void make_unique_constraint(std::string_view table_name,
                                    std::vector<std::shared_ptr<FieldBase>> conflict_fields) override;
        void setup_full_text_search(
            std::string_view table_name,
            std::vector<std::shared_ptr<FieldBase>> fields) override;
        void drop_full_text_search(std::string_view table_name) const;
        void remove_full_text_search(std::string_view table_name);
        void restore_full_text_search(std::string_view table_name) const;
        // Table Management
        void create_table(std::string_view table_name, const Record& field_list) override;
        void remove_table(std::string_view table_name) override;
        [[nodiscard]] bool check_table(std::string_view table_name) override;

        // Data Retrieval
        [[nodiscard]] std::vector<Record> select(
            std::string_view table_name,
            const FieldConditions& conditions) const override;
        [[nodiscard]] std::vector<std::unique_ptr<ViewRecord>> view(
            std::string_view table_name,
            const FieldConditions& conditions) const override;
        [[nodiscard]] std::vector<Record> select(
            std::string_view table_name) const override;
        [[nodiscard]] std::vector<std::unique_ptr<ViewRecord>> view(std::string_view table_name) const override;
        // Remove Data
        void remove(
            std::string_view table_name,
            const FieldConditions& conditions) override;
        void truncate(
            std::string_view table_name) override;
        // Get Record Count
        [[nodiscard]] uint32_t count(std::string_view table_name,
                                     const FieldConditions& conditions) const override;
        [[nodiscard]] uint32_t count(std::string_view table_name) const override;
        // Full-Text Search Methods
        [[nodiscard]] std::vector<Record> get_data_fts(
            std::string_view table_name,
            const std::string& fts_query_params) const override;

    protected:
        // Implementation Methods for Data Manipulation
        void insert_implementation(std::string_view table_name, const std::vector<Record>& rows) override;
        void insert_implementation(std::string_view table_name, std::vector<Record>&& rows) override;

        void upsert_implementation(std::string_view table_name,
                                   const std::vector<Record>& rows,
                                   const std::vector<std::shared_ptr<FieldBase>>& replace_fields) override;

        void upsert_implementation(std::string_view table_name,
                                   std::vector<Record>&& rows,
                                   const std::vector<std::shared_ptr<FieldBase>>& replace_fields) override;

    private:
        boost::container::flat_map<uint32_t, std::string> type_oids_; // id, name
        boost::container::flat_map<std::string, std::vector<std::shared_ptr<FieldBase>>> conflict_fields_ = {};
        boost::container::flat_map<std::string, std::vector<std::shared_ptr<FieldBase>>> fts_fields_ = {};
        std::shared_ptr<pqxx::connection> conn_;
        mutable std::recursive_mutex conn_mutex_;
        mutable std::unique_ptr<pqxx::work> open_transaction_;
        bool in_transaction_;

        void _oid_preprocess();
        [[nodiscard]] std::unique_ptr<FieldBase> process_field(const pqxx::field& field) const;
        // Utility Methods
        [[nodiscard]] static bool is_valid_identifier(std::string_view identifier);

        void execute_query(const std::string& query_string, const pqxx::params& params) const;
        void execute_query(const std::string& query_string) const;
        pqxx::result execute_query_with_result(const std::string& query_string, const pqxx::params& params) const;
        pqxx::result execute_query_with_result(const std::string& query_string) const;

        template <interfaces::RecordContainer Rec>
        std::pair<std::string, pqxx::params> construct_insert_query(
            const std::string_view table_name,
            Rec&& rows) const
        {
            if (rows.empty())
            {
                throw exceptions::QueryException("No data provided for insert.", errors::db_error_code::INVALID_QUERY);
            }
            const std::string table = escape_identifier(table_name);
            std::ostringstream query_stream;
            query_stream << "INSERT INTO " << table << " (";
            // Get field names from the first record
            const auto& first_record = rows.front();
            std::vector<std::string> field_names;
            for (const auto& it : first_record)
            {
                field_names.push_back(it->get_name());
                query_stream << escape_identifier(field_names.back()) << ", ";
            }
            std::string query = query_stream.str();
            query.erase(query.size() - 2); // Remove last comma and space
            query += ") VALUES ";


            pqxx::params params;
            uint32_t param_counter = 1;
            for (auto&& record : std::forward<Rec>(rows))
            {
                query += "(";
                for (auto&& field : record)
                {
                    query += "$" + std::to_string(param_counter++) + ", ";

                    // Call the correct version of to_string() depending on the value category
                    params.append(std::forward<decltype(field)>(field)->to_string());
                }
                query.erase(query.size() - 2); // Remove last comma and space
                query += "), ";
            }
            query.erase(query.size() - 2); // Remove last comma and space
            query += ";";
            return {query, params};
        }

        std::string escape_identifier(std::string_view identifier) const;

        std::unique_ptr<pqxx::work> initialize_transaction() const;
        void finish_transaction(std::unique_ptr<pqxx::work>&& current_transaction) const;
        static exceptions::DatabaseException adapt_exception(const std::exception& pqxxerr);

        void build_conflict_clause(std::string&, std::string_view table_name,
                                   const std::vector<std::shared_ptr<FieldBase>>&) const;
        void create_fts_index_query(std::string_view table_name, std::ostringstream& index_query) const;
        static std::string make_fts_index(std::string_view table_name);
    };
}
