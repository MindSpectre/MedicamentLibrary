// pqxx_client.cpp

#include "pqxx_client.hpp"
#include <sstream>
#include <chrono>
#include <iostream>
#include <regex>
#include "db_field.hpp"
#include "db_conditions.hpp"

namespace drug_lib::common::database
{
    using namespace exceptions;

    PqxxClient::PqxxClient(std::string_view host,
                           int port,
                           std::string_view db_name,
                           std::string_view login,
                           std::string_view password)
        : in_transaction_(false)
    {
        try
        {
            std::ostringstream conn_str;
            conn_str << "host=" << host
                << " port=" << port
                << " user=" << login
                << " password=" << password
                << " dbname=" << db_name;

            conn_ = std::make_shared<pqxx::connection>(conn_str.str());

            if (!conn_->is_open())
            {
                throw ConnectionException("Failed to open database connection.");
            }
        }
        catch (const pqxx::sql_error& e)
        {
            throw ConnectionException(e.what());
        }
        catch (const std::exception& e)
        {
            throw ConnectionException(e.what());
        }
    }

    // Utility method to check if an identifier is valid
    bool PqxxClient::is_valid_identifier(std::string_view identifier)
    {
        static const std::regex valid_identifier_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
        return std::regex_match(identifier.begin(), identifier.end(), valid_identifier_regex);
    }

    // Utility method to escape identifiers
    std::string PqxxClient::escape_identifier(std::string_view identifier) const
    {
        if (!is_valid_identifier(identifier))
        {
            throw InvalidIdentifierException(std::string(identifier));
        }
        return conn_->quote_name(std::string(identifier));
    }

    // Utility method to execute a query
    void PqxxClient::execute_query(const std::string& query_string, const pqxx::params& params) const
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        try
        {
            pqxx::work txn(*conn_);
            txn.exec_params(query_string, params);
            txn.commit();
        }
        catch (const pqxx::sql_error& e)
        {
            throw QueryException(e.what());
        }
    }

    // Transaction Methods
    void PqxxClient::start_transaction()
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        if (in_transaction_)
        {
            throw TransactionException("Transaction already started.");
        }
        try
        {
            pqxx::work txn(*conn_);
            txn.exec("BEGIN;");
            txn.commit();
            in_transaction_ = true;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what());
        }
    }

    void PqxxClient::commit_transaction()
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        if (!in_transaction_)
        {
            throw TransactionException("No active transaction to commit.");
        }
        try
        {
            pqxx::work txn(*conn_);
            txn.exec("COMMIT;");
            txn.commit();
            in_transaction_ = false;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what());
        }
    }

    void PqxxClient::rollback_transaction()
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        if (!in_transaction_)
        {
            throw TransactionException("No active transaction to rollback.");
        }
        try
        {
            pqxx::work txn(*conn_);
            txn.exec("ROLLBACK;");
            txn.commit();
            in_transaction_ = false;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what());
        }
    }

    // Table Management
    void PqxxClient::create_table(std::string_view table_name, const Record& field_list)
    {
        std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "CREATE TABLE " << table << " (";

        for (const auto& [field_name, field_value] : field_list)
        {
            std::string name = escape_identifier(field_name);
            std::string type = field_value->get_sql_type();
            query_stream << name << " " << type << ", ";
        }

        std::string query = query_stream.str();
        query.erase(query.size() - 2); // Remove last comma and space
        query += ");";

        execute_query(query, pqxx::params{});
    }

    void PqxxClient::remove_table(std::string_view table_name)
    {
        const std::string table = escape_identifier(table_name);
        const std::string query = "DROP TABLE IF EXISTS " + table + ";";
        execute_query(query, pqxx::params{});
    }

    bool PqxxClient::check_table(std::string_view table_name) const
    {
        const std::string table = conn_->esc(std::string(table_name));
        const std::string query = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = " + conn_->
            quote(table) + ");";
        std::lock_guard<std::mutex> lock(conn_mutex_);
        try
        {
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec(query);
            return res[0][0].as<bool>();
        }
        catch (const pqxx::sql_error& e)
        {
            throw QueryException(e.what());
        }
    }

    // Data Manipulation Implementation
    void PqxxClient::add_data_impl(const std::string_view table_name, const std::vector<Record>& rows)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        execute_query(query, params);
    }

    void PqxxClient::add_data_impl(const std::string_view table_name, std::vector<Record>&& rows)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        execute_query(query, params);
    }

    // Upsert Data Implementation
    void PqxxClient::upsert_data_impl(const std::string_view table_name,
                                      const std::vector<Record>& rows,
                                      const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                                      const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
    {
        auto [query, params] = construct_insert_query(table_name, rows);

        // Build ON CONFLICT clause
        std::ostringstream conflict_stream;
        conflict_stream << " ON CONFLICT (";
        for (const auto& field : conflict_fields)
        {
            conflict_stream << escape_identifier(field->get_name()) << ", ";
        }
        std::string conflict_clause = conflict_stream.str();
        conflict_clause.erase(conflict_clause.size() - 2); // Remove last comma and space
        conflict_clause += ") DO UPDATE SET ";

        // Build SET clause
        for (const auto& field : replace_fields)
        {
            std::string name = escape_identifier(field->get_name());
            conflict_clause.append(name).append(" = EXCLUDED.").append(name).append(", ");
        }
        conflict_clause.erase(conflict_clause.size() - 2); // Remove last comma and space

        query += conflict_clause + ";";
        execute_query(query, params);
    }

    void PqxxClient::upsert_data_impl(const std::string_view table_name,
                                      std::vector<Record>&& rows,
                                      const std::vector<std::shared_ptr<FieldBase>>& conflict_fields,
                                      const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
    {
        upsert_data_impl(table_name, rows, conflict_fields, replace_fields);
    }

    // Data Retrieval
    std::vector<Record> PqxxClient::get_data(
        std::string_view table_name,
        const FieldConditions& conditions) const
    {
        std::vector<Record> results;
        std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "SELECT * FROM " << table;

        if (!conditions.empty())
        {
            pqxx::params params;
            query_stream << " WHERE ";
            int param_index = 1;
            for (const auto& condition : conditions)
            {
                std::string field_name = escape_identifier(condition.field()->get_name());
                query_stream << field_name << " " << condition.op() << " $" << param_index++ << " AND ";
                params.append(condition.value()->to_string());
            }
            std::string query = query_stream.str();
            query.erase(query.size() - 5); // Remove last ' AND '
            query += ";";
            std::lock_guard<std::mutex> lock(conn_mutex_);
            try
            {
                pqxx::work txn(*conn_);
                pqxx::result res = txn.exec_params(query, params);
                for (const auto& row : res)
                {
                    Record record;
                    for (const auto& field : row)
                    {
                        auto field_ptr = std::make_shared<Field<std::string>>(field.name(), field.c_str());
                        record.add_field(field_ptr);
                    }
                    results.push_back(std::move(record));
                }
            }
            catch (const pqxx::sql_error& e)
            {
                throw QueryException(e.what());
            }
        }
        else
        {
            // No conditions, select all
            std::string query = query_stream.str() + ";";
            std::lock_guard<std::mutex> lock(conn_mutex_);
            try
            {
                pqxx::work txn(*conn_);
                pqxx::result res = txn.exec(query);
                for (const auto& row : res)
                {
                    Record record;
                    for (const auto& field : row)
                    {
                        auto field_ptr = std::make_shared<Field<std::string>>(field.name(), field.c_str());
                        record.add_field(field_ptr);
                    }
                    results.push_back(std::move(record));
                }
            }
            catch (const pqxx::sql_error& e)
            {
                throw QueryException(e.what());
            }
        }

        return results;
    }

    // Remove Data
    void PqxxClient::remove_data(
        const std::string_view table_name,
        const FieldConditions& conditions)
    {
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "DELETE FROM " << table;

        if (!conditions.empty())
        {
            pqxx::params params;
            query_stream << " WHERE ";
            int param_index = 1;
            for (const auto& condition : conditions)
            {
                std::string field_name = escape_identifier(condition.field()->get_name());
                query_stream << field_name << " " << condition.op() << " $" << param_index++ << " AND ";
                params.append(condition.value()->to_string());
            }
            std::string query = query_stream.str();
            query.erase(query.size() - 5); // Remove last ' AND '
            query += ";";
            execute_query(query, params);
        }
        else
        {
            // No conditions provided, throw exception to prevent deleting all data
            throw QueryException("No conditions provided for delete operation.");
        }
    }

    // Get Record Count
    int PqxxClient::get_count(const std::string_view table_name,
                              const std::shared_ptr<FieldBase>& field,
                              std::chrono::duration<double>& query_exec_time) const
    {
        const std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::high_resolution_clock::now();
        const std::string table = escape_identifier(table_name);
        const std::string field_name = escape_identifier(field->get_name());
        const std::string query = "SELECT COUNT(" + field_name + ") FROM " + table + ";";
        std::lock_guard<std::mutex> lock(conn_mutex_);
        try
        {
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec(query);
            const std::chrono::time_point<std::chrono::system_clock> end_time =
                std::chrono::high_resolution_clock::now();
            query_exec_time = end_time - start_time;
            return res[0][0].as<int>();
        }
        catch (const pqxx::sql_error& e)
        {
            throw QueryException(e.what());
        }
    }

    // Full-Text Search Methods
    std::vector<Record> PqxxClient::get_data_fts(
        const std::string_view table_name,
        const std::string_view fts_query_params,
        std::chrono::duration<double>& query_exec_time) const
    {
        const std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::high_resolution_clock::now();
        std::vector<Record> results;
        const std::string table = escape_identifier(table_name);
        const std::string query = "SELECT * FROM " + table +
            " WHERE to_tsvector('simple', json_data::text) @@ to_tsquery('simple', $1);";
        pqxx::params params;
        params.append(std::string(fts_query_params));
        std::lock_guard<std::mutex> lock(conn_mutex_);
        try
        {
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec_params(query, params);
            const std::chrono::time_point<std::chrono::system_clock> end_time =
                std::chrono::high_resolution_clock::now();
            query_exec_time = end_time - start_time;
            for (const auto& row : res)
            {
                Record record;
                for (const auto& field : row)
                {
                    auto field_ptr = std::make_shared<Field<std::string>>(field.name(), field.c_str());
                    record.add_field(field_ptr);
                }
                results.push_back(std::move(record));
            }
        }
        catch (const pqxx::sql_error& e)
        {
            throw QueryException(e.what());
        }

        return results;
    }

    bool PqxxClient::get_data_fts_batched(
        const std::string_view table_name,
        const std::string_view fts_query_params,
        std::chrono::duration<double>& query_exec_time,
        const std::function<void(const std::vector<Record>&)>& on_result) const
    {
        const std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::high_resolution_clock::now();
        const std::string table = escape_identifier(table_name);
        const std::string query = "SELECT * FROM " + table +
            " WHERE to_tsvector('simple', json_data::text) @@ to_tsquery('simple', $1);";
        pqxx::params params;
        params.append(std::string(fts_query_params));
        std::lock_guard<std::mutex> lock(conn_mutex_);
        try
        {
            std::vector<Record> batch_results;
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec_params(query, params);
            const std::chrono::time_point<std::chrono::system_clock> end_time =
                std::chrono::high_resolution_clock::now();
            query_exec_time = end_time - start_time;

            for (const auto& row : res)
            {
                Record record;
                for (const auto& field : row)
                {
                    auto field_ptr = std::make_shared<Field<std::string>>(field.name(), field.c_str());
                    record.add_field(field_ptr);
                }
                batch_results.push_back(std::move(record));

                if (batch_results.size() >= 8)
                {
                    on_result(batch_results);
                    batch_results.clear();
                }
            }

            if (!batch_results.empty())
            {
                on_result(batch_results);
            }

            return true;
        }
        catch (const pqxx::sql_error& e)
        {
            throw QueryException(e.what());
        }
    }

    // Construct Insert Query
    std::pair<std::string, pqxx::params> PqxxClient::construct_insert_query(
        const std::string_view table_name,
        const std::vector<Record>& rows) const
    {
        if (rows.empty())
        {
            throw QueryException("No data provided for insert.");
        }

        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "INSERT INTO " << table << " (";

        // Get field names from the first record
        const auto& first_record = rows.front();
        std::vector<std::string> field_names;
        for (const auto& [field_name, _] : first_record)
        {
            field_names.push_back(field_name);
            query_stream << escape_identifier(field_names.back()) << ", ";
        }
        std::string query = query_stream.str();
        query.erase(query.size() - 2); // Remove last comma and space
        query += ") VALUES ";

        pqxx::params params;
        int param_counter = 1;
        for (const auto& record : rows)
        {
            query += "(";
            for (const auto& field_name : field_names)
            {
                query += "$" + std::to_string(param_counter++) + ", ";
                params.append(record.at(field_name)->to_string());
            }
            query.erase(query.size() - 2); // Remove last comma and space
            query += "), ";
        }
        query.erase(query.size() - 2); // Remove last comma and space
        query += ";";
        return {query, params};
    }

    std::pair<std::string, pqxx::params> PqxxClient::construct_insert_query(
        const std::string_view table_name,
        std::vector<Record>&& rows) const
    {
        return construct_insert_query(table_name, rows);
    }
}
