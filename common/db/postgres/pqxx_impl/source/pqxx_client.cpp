// pqxx_client.cpp

#include "pqxx_client.hpp"

#include <chrono>
#include <iostream>
#include <regex>
#include <sstream>

#include "db_conditions.hpp"
#include "db_field.hpp"

namespace drug_lib::common::database
{
    using namespace exceptions;
    using db_err = errors::db_error_code;

    void PqxxClient::create_database(std::string_view host, int port, std::string_view db_name, std::string_view login,
                                     std::string_view password)
    // Build the connection string to connect to the 'template1' database
    {
        std::ostringstream conn_str;
        conn_str << "host=" << host
            << " port=" << port
            << " user=" << login
            << " password=" << password
            << " dbname=template1"; // Connect to 'template1' to create a new database

        try
        {
            // Establish connection to the 'template1' database

            if (pqxx::connection trivial(conn_str.str()); trivial.is_open())
            {
                // Start a non-transactional query to create the database
                pqxx::nontransaction db_creating(trivial);

                // Prepare the SQL query
                std::ostringstream query;
                query << "CREATE DATABASE " << db_name
                    << " WITH OWNER = " << login << " "
                    << "ENCODING = 'UTF8' "
                    << "TEMPLATE template0;"; // Use template0 for a clean database

                // Execute the query
                db_creating.exec(query.str());
                db_creating.commit();

                std::cout << "Database " << db_name << " created successfully!" << std::endl;
            }
            else
            {
                std::cerr << "Connection to the database failed." << std::endl;
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error creating database: " << e.what() << std::endl;
            throw; // Rethrow the exception if needed
        }
    }


    void PqxxClient::create_database(const PqxxConnectParams& pr)
    {
        create_database(pr.get_host(), pr.get_port(), pr.get_login(), pr.get_password(), pr.get_db_name());
    }

    void PqxxClient::drop_connect()
    {
        conn_->close();
    }

    PqxxClient::PqxxClient(const std::string_view host,
                           const int port,
                           const std::string_view db_name,
                           const std::string_view login,
                           const std::string_view password)
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
                throw ConnectionException("Failed to open database connection.",
                                          db_err::CONNECTION_FAILED);
            }
            _oid_preprocess();
        }
        catch (const pqxx::too_many_connections& e)
        {
            throw ConnectionException(e.what(), db_err::CONNECTION_POOL_EXHAUSTED);
        }
        catch (const pqxx::sql_error& e)
        {
            throw ConnectionException(e.what(), db_err::INVALID_QUERY);
        }

        catch (const std::exception& e)
        {
            throw ConnectionException(e.what(), db_err::PERMISSION_DENIED);
        }
    }


    PqxxClient::PqxxClient(const PqxxConnectParams& pr)
        : PqxxClient(pr.get_host(), pr.get_port(), pr.get_login(), pr.get_password(), pr.get_db_name())
    {
    }

    // Utility method to check if an identifier is valid
    bool PqxxClient::is_valid_identifier(const std::string_view identifier)
    {
        static const std::regex valid_identifier_regex("^[a-zA-Z_][a-zA-Z0-9_]*$");
        return std::regex_match(identifier.begin(), identifier.end(), valid_identifier_regex);
    }

    // Utility method to escape identifiers
    std::string PqxxClient::escape_identifier(const std::string_view identifier) const
    {
        if (!is_valid_identifier(identifier))
        {
            throw InvalidIdentifierException(std::string(identifier), db_err::INVALID_QUERY);
        }
        return conn_->quote_name(std::string(identifier));
    }

    std::shared_ptr<pqxx::work> PqxxClient::initialize_transaction()
    {
        if (in_transaction_)
        {
            return shared_transaction_;
        }

        try
        {
            auto one_query_transaction = std::make_shared<pqxx::work>(*conn_);
            return one_query_transaction;
        }
        catch (std::exception& e)
        {
            throw TransactionException(std::string(e.what()).append("UNEXPECTED WORK THROWS ERROR"),
                                       db_err::TRANSACTION_START_FAILED);
        }
    }

    void PqxxClient::finish_transaction(const std::shared_ptr<pqxx::work>& current_transaction) const
    {
        if (in_transaction_)
        {
            return;
        }
        try
        {
            current_transaction->commit();
        }
        catch (std::exception& e)
        {
            throw TransactionException(std::string(e.what()).append("UNEXPECTED WORK THROWS ERROR"),
                                       db_err::TRANSACTION_START_FAILED);
        }
    }

    DatabaseException PqxxClient::adapt_exception(const std::exception& pqxxerr)
    {
        if (const auto* deadlock_error = dynamic_cast<const pqxx::deadlock_detected*>(&pqxxerr))
        {
            return DatabaseException(deadlock_error->what(), db_err::DEADLOCK_DETECTED);
        }

        if (const auto* rollback_error = dynamic_cast<const pqxx::transaction_rollback*>(&pqxxerr))
        {
            return DatabaseException(rollback_error->what(), db_err::SYSTEM_ROLLBACK);
        }

        if (const auto* syntax_error = dynamic_cast<const pqxx::syntax_error*>(&pqxxerr))
        {
            return DatabaseException(syntax_error->what(), db_err::INVALID_QUERY);
        }

        if (const auto* data_error = dynamic_cast<const pqxx::data_exception*>(&pqxxerr))
        {
            return DatabaseException(data_error->what(), db_err::INVALID_DATA);
        }

        if (const auto* sql_error = dynamic_cast<const pqxx::sql_error*>(&pqxxerr))
        {
            return DatabaseException(sql_error->what(), db_err::QUERY_EXECUTION_FAILED);
        }

        // If none of the specific exceptions match, return a generic exception
        return DatabaseException(pqxxerr.what(), db_err::UNKNOWN_ERROR);
    }

    void PqxxClient::build_conflict_clause(std::string& query,
                                           const std::vector<std::shared_ptr<FieldBase>>& replace_fields) const
    {
        query.pop_back();
        // Build ON CONFLICT clause
        std::ostringstream conflict_stream;
        conflict_stream << " ON CONFLICT (";
        for (const auto& field : conflict_fields_)
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
    }

    pqxx::params PqxxClient::build_params_impl(std::string& query, const std::vector<Record>& rows,
                                               const std::vector<std::string>& field_names)
    {
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
        return params;
    }

    pqxx::params PqxxClient::build_params_impl(std::string& query, std::vector<Record>&& rows,
                                               const std::vector<std::string>& field_names)
    {
        pqxx::params params;
        int param_counter = 1;
        for (const auto& record : rows)
        {
            query += "(";
            for (const auto& field_name : field_names)
            {
                query += "$" + std::to_string(param_counter++) + ", ";
                params.append(record.at(field_name)->move_to_string());
            }
            query.erase(query.size() - 2); // Remove last comma and space
            query += "), ";
        }
        query.erase(query.size() - 2); // Remove last comma and space
        query += ";";
        return params;
    }


    // Utility method to execute a query
    void PqxxClient::execute_query(const std::string& query_string, const pqxx::params& params)
    {
        std::lock_guard lock(conn_mutex_);
        try
        {
            const auto txn = initialize_transaction();
            txn->exec_params(query_string, params);
            finish_transaction(txn);
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    // Transaction Methods
    void PqxxClient::start_transaction()
    {
        std::lock_guard lock(conn_mutex_);
        if (in_transaction_)
        {
            throw TransactionException("Transaction already started.", db_err::TRANSACTION_START_FAILED);
        }
        try
        {
            shared_transaction_ = std::make_shared<pqxx::work>(*conn_);
            in_transaction_ = true;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what(), db_err::QUERY_EXECUTION_FAILED);
        }
    }

    void PqxxClient::commit_transaction()
    {
        std::lock_guard lock(conn_mutex_);
        if (!in_transaction_)
        {
            throw TransactionException("No active transaction to commit.", db_err::TRANSACTION_COMMIT_FAILED);
        }
        try
        {
            shared_transaction_->commit();
            shared_transaction_.reset();
            in_transaction_ = false;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what(), db_err::QUERY_EXECUTION_FAILED);
        }
    }

    void PqxxClient::rollback_transaction()
    {
        std::lock_guard lock(conn_mutex_);
        if (!in_transaction_)
        {
            throw TransactionException("No active transaction to rollback.", db_err::TRANSACTION_ROLLBACK_FAILED);
        }
        try
        {
            shared_transaction_->abort();
            shared_transaction_.reset();
            in_transaction_ = false;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what(), db_err::QUERY_EXECUTION_FAILED);
        }
    }

    void PqxxClient::make_unique_constraint(const std::string_view table_name,
                                            std::vector<std::shared_ptr<FieldBase>>&& conflict_fields)
    {
        conflict_fields_ = std::move(conflict_fields);
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        std::ostringstream sub_query;
        query_stream << "ALTER TABLE " << table << " ADD CONSTRAINT ";
        std::ostringstream name_constraint;
        sub_query << " UNIQUE (";
        for (auto&& column : conflict_fields_)
        {
            sub_query << escape_identifier(column->get_name()) << ",";
            name_constraint << column->get_name() << "_";
        }
        name_constraint << table_name;
        std::string query = query_stream.str();
        query.append(escape_identifier(name_constraint.str())).append(sub_query.str());
        query.pop_back();
        query += ");";

        execute_query(query, pqxx::params{});
    }

    // Table Management
    void PqxxClient::create_table(const std::string_view table_name, const Record& field_list)
    {
        const std::string table = escape_identifier(table_name);
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

    void PqxxClient::remove_table(const std::string_view table_name)
    {
        const std::string table = escape_identifier(table_name);
        const std::string query = "DROP TABLE IF EXISTS " + table + ";";
        execute_query(query, pqxx::params{});
    }

    bool PqxxClient::check_table(const std::string_view table_name) const
    {
        const std::string table = conn_->esc(std::string(table_name));
        const std::string query = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = " + conn_->
            quote(table) + ");";
        std::lock_guard lock(conn_mutex_);
        try
        {
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec(query);
            return res[0][0].as<bool>();
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
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
                                      const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        build_conflict_clause(query, replace_fields);
        execute_query(query, params);
    }

    void PqxxClient::upsert_data_impl(const std::string_view table_name,
                                      std::vector<Record>&& rows,
                                      const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        build_conflict_clause(query, replace_fields);
        execute_query(query, params);
    }

    void PqxxClient::setup_full_text_search(
        std::string_view table_name,
        std::vector<std::shared_ptr<FieldBase>> fields)
    {
        if (fields.empty())
        {
            throw std::invalid_argument("Field list for full-text search cannot be empty.");
        }
        fts_fields_ = std::move(fields);
        std::lock_guard lock(conn_mutex_);

        try
        {
            const std::string table = escape_identifier(table_name);

            // Build the concatenated fields expression
            std::ostringstream fields_stream;
            for (const auto& field : fts_fields_)
            {
                fields_stream << "coalesce(" << escape_identifier(field->get_name()) << "::text, '') || ' ' || ";
            }
            std::string fields_concatenated = fields_stream.str();
            fields_concatenated.erase(fields_concatenated.size() - 11); // Remove last " || ' ' || "

            // Create the functional index
            std::ostringstream index_query;
            index_query << "CREATE INDEX IF NOT EXISTS idx_" << table_name << "_fts ON " << table
                << " USING gin (to_tsvector('simple', " << fields_concatenated << "));";

            execute_query(index_query.str(), pqxx::params{});
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    void PqxxClient::_oid_preprocess()
    {
        try
        {
            pqxx::nontransaction txn(*conn_);
            const pqxx::result r = txn.exec(
                "SELECT typname, oid FROM pg_type WHERE typname IN ('bool', 'int2', 'int4', 'int8', 'float4', 'float8', 'text', 'varchar', 'bpchar', 'timestamp', 'timestamptz')");
            for (const auto& row : r)
            {
                type_oids_.insert({row["typname"].c_str(), row["oid"].as<int>()});
            }
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    std::shared_ptr<FieldBase> PqxxClient::process_field(pqxx::field&& field) const
    {
        std::shared_ptr<FieldBase> field_ptr;

        // Get the field's PostgreSQL type OID
        const auto type_oid = field.type();
        // Determine the C++ type based on the PostgreSQL type OID and create the appropriate Field object
        if (type_oid == type_oids_.at("int4")) // 32-bit integer
        {
            auto value = field.as<int32_t>();
            field_ptr = std::make_shared<Field<int>>(field.name(), value);
            return field_ptr;
        }

        if (type_oid == type_oids_.at("int8")) // 64-bit integer (Bigint)
        {
            auto value = field.as<int64_t>();
            field_ptr = std::make_shared<Field<int64_t>>(field.name(), value);
            return field_ptr;
        }

        // Handle float types
        if (type_oid == type_oids_.at("float8")) // Double precision (64-bit float)
        {
            auto value = field.as<double>();
            field_ptr = std::make_shared<Field<double>>(field.name(), value);
            return field_ptr;
        }

        // Handle string types (text, varchar, char)
        if (type_oid == type_oids_.at("text")) // Text type
        {
            auto value = field.as<std::string>();
            field_ptr = std::make_shared<Field<std::string>>(field.name(), std::move(value));
            return field_ptr;
        }

        // Handle boolean type
        if (type_oid == type_oids_.at("bool")) // Boolean type
        {
            auto value = field.as<bool>();
            field_ptr = std::make_shared<Field<bool>>(field.name(), value);
            return field_ptr;
        }

        // Handle timestamp types (with or without timezone)
        if (type_oid == type_oids_.at("timestamptz") || type_oid == type_oids_.at("timestamp")) // Timestamp type
        {
            const auto value = field.as<std::string>();
            // Parse the timestamp string into std::chrono::system_clock::time_point
            std::istringstream iss(value);
            std::tm tm = {};
            iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // Parse the string to fill the tm structure
            auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm)); // Convert to time_point
            field_ptr = std::make_shared<Field<std::chrono::system_clock::time_point>>(field.name(), tp);
            return field_ptr;
        }

        // Return the field pointer (nullptr if no matching type was found)
        return field_ptr;
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
        std::string query;
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
            query = query_stream.str();
            query.erase(query.size() - 5); // Remove last ' AND '
            query += ";";
        }
        else
        {
            // No conditions, select all
            query = query_stream.str() + ";";
        }
        std::lock_guard lock(conn_mutex_);
        try
        {
            pqxx::nontransaction txn(*conn_);
            for (pqxx::result res = txn.exec(query); auto&& row : res)
            {
                Record record;
                for (auto&& field : row)
                {
                    auto field_ptr = process_field(std::move(field));
                    record.add_field(std::move(field_ptr));
                }
                results.push_back(std::move(record));
            }
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
        return results;
    }

    // Remove Data
    // TODO: Possibly should append moveable
    void PqxxClient::remove_data(
        const std::string_view table_name,
        const FieldConditions& conditions)
    {
        const std::string table = escape_identifier(table_name);

        std::ostringstream query_stream;
        pqxx::params params;
        std::string query;
        if (!conditions.empty())
        {
            query_stream << "DELETE FROM " << table;

            query_stream << " WHERE ";
            int param_index = 1;
            for (const auto& condition : conditions)
            {
                std::string field_name = escape_identifier(condition.field()->get_name());
                query_stream << field_name << " " << condition.op() << " $" << param_index++ << " AND ";
                params.append(condition.value()->to_string());
            }
            query = query_stream.str();
            query.erase(query.size() - 5); // Remove last ' AND '
            query += ";";
        }
        else
        {
            query_stream << "TRUNCATE TABLE " << table << ";";
            query = query_stream.str();
        }
        execute_query(query, params);
    }

    //TODO: gotta be review with field conditions

    // Get Record Count
    int PqxxClient::get_count(const std::string_view table_name,
                              const std::shared_ptr<FieldBase>& field,
                              std::chrono::duration<double>& query_exec_time) const
    {
        const std::chrono::time_point<std::chrono::system_clock> start_time =
            std::chrono::high_resolution_clock::now();
        const std::string table = escape_identifier(table_name);
        const std::string field_name = escape_identifier(field->get_name());
        const std::string query = "SELECT COUNT(" + field_name + ") FROM " + table + ";";
        std::lock_guard lock(conn_mutex_);
        try
        {
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec(query);
            const std::chrono::time_point<std::chrono::system_clock> end_time =
                std::chrono::high_resolution_clock::now();
            query_exec_time = end_time - start_time;
            return res[0][0].as<int>();
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    // Full-Text Search Methods
    std::vector<Record> PqxxClient::get_data_fts(
        std::string_view table_name,
        std::string_view fts_query_params,
        std::chrono::duration<double>& query_exec_time) const
    {
        const auto start_time = std::chrono::high_resolution_clock::now();
        std::vector<Record> results;
        const std::string table = escape_identifier(table_name);

        // Build the concatenated fields expression
        std::ostringstream fields_stream;
        for (const auto& field : fts_fields_)
        {
            fields_stream << "coalesce(" << field->get_name() << "::text, '') || ' ' || ";
        }
        std::string fields_str = fields_stream.str();
        fields_str.erase(fields_str.size() - 11); // Remove last " || ' ' || "

        const std::string query = "SELECT * FROM " + table +
            " WHERE to_tsvector('simple', " + fields_str + ") @@ to_tsquery('simple', $1);";

        pqxx::params params;
        params.append(std::string(fts_query_params));

        std::lock_guard lock(conn_mutex_);
        try
        {
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec_params(query, params);
            query_exec_time = std::chrono::high_resolution_clock::now() - start_time;

            for (const auto& row : res)
            {
                Record record;
                for (auto&& field : row)
                {
                    auto field_ptr = process_field(std::move(field));
                    record.add_field(std::move(field_ptr));
                }
                results.push_back(std::move(record));
            }
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }

        return results;
    }


    //WARNING: UNUSED
    bool PqxxClient::get_data_fts_batched(
        const std::string_view table_name,
        const std::string_view fts_query_params,
        std::chrono::duration<double>& query_exec_time,
        const std::function<void(const std::vector<Record>&)>& on_result) const
    {
        const std::chrono::time_point<std::chrono::system_clock> start_time =
            std::chrono::high_resolution_clock::now();
        const std::string table = escape_identifier(table_name);
        const std::string query = "SELECT * FROM " + table +
            " WHERE to_tsvector('simple', json_data::text) @@ to_tsquery('simple', $1);";
        pqxx::params params;
        params.append(std::string(fts_query_params));
        std::lock_guard lock(conn_mutex_);
        try
        {
            std::vector<Record> batch_results;
            pqxx::nontransaction ntx(*conn_);
            const pqxx::result res = ntx.exec_params(query, params);
            const std::chrono::time_point<std::chrono::system_clock> end_time =
                std::chrono::high_resolution_clock::now();
            query_exec_time = end_time - start_time;

            for (auto&& row : res)
            {
                Record record;
                for (auto&& field : row)
                {
                    auto field_ptr = process_field(std::move(field));
                    record.add_field(std::move(field_ptr));
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
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }
}
