// pqxx_client.cpp

#include "pqxx_client.hpp"

#include <chrono>
#include <iostream>
#include <regex>
#include <sstream>

#include "db_conditions.hpp"
#include "db_field.hpp"
#include "pqxx_view_record.hpp"
#include "stopwatch.hpp"

namespace drug_lib::common::database
{
    using namespace exceptions;
    using db_err = errors::db_error_code;
    //Constructors
    PqxxClient::PqxxClient(const std::string_view host,
                           const uint32_t port,
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
        : in_transaction_(false)
    {
        conn_ = std::make_shared<pqxx::connection>(pr.make_connect_string());
        if (!conn_->is_open())
        {
            throw ConnectionException("Failed to open database connection.",
                                      db_err::CONNECTION_FAILED);
        }
        _oid_preprocess();
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

    // Static create postgreSQL Database
    void PqxxClient::create_database(std::string_view host, uint32_t port, std::string_view db_name,
                                     std::string_view login,
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

    //
    void PqxxClient::drop_connect()
    {
        conn_->close();
    }


    std::unique_ptr<pqxx::work> PqxxClient::initialize_transaction() const
    {
        if (in_transaction_)
        {
            return std::move(open_transaction_);
        }

        try
        {
            auto one_query_transaction = std::make_unique<pqxx::work>(*conn_);
            return one_query_transaction;
        }
        catch (std::exception& e)
        {
            throw TransactionException(std::string(e.what()).append("UNEXPECTED WORK THROWS ERROR"),
                                       db_err::TRANSACTION_START_FAILED);
        }
    }

    void PqxxClient::finish_transaction(std::unique_ptr<pqxx::work>&& current_transaction) const
    {
        if (in_transaction_)
        {
            open_transaction_ = std::move(current_transaction);
            return;
        }
        try
        {
            current_transaction->commit();
            current_transaction.reset();
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
                                           const std::string_view table_name,
                                           const std::vector<std::shared_ptr<FieldBase>>& replace_fields) const
    {
        query.pop_back();
        // Build ON CONFLICT clause
        std::ostringstream conflict_stream;
        const auto conflict_fields = conflict_fields_.at(table_name.data());
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
    }


    // Utility method to execute a query
    void PqxxClient::execute_query(const std::string& query_string, const pqxx::params& params) const
    {
        std::lock_guard lock(conn_mutex_);
        try
        {
            std::unique_ptr<pqxx::work> txn = initialize_transaction();
            txn->exec_params(query_string, params);
            finish_transaction(std::move(txn));
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    void PqxxClient::execute_query(const std::string& query_string) const
    {
        std::lock_guard lock(conn_mutex_);
        try
        {
            std::unique_ptr<pqxx::work> txn = initialize_transaction();
            txn->exec(query_string);
            finish_transaction(std::move(txn));
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    pqxx::result PqxxClient::execute_query_with_result(const std::string& query_string,
                                                       const pqxx::params& params) const
    {
        std::lock_guard lock(conn_mutex_);
        try
        {
            std::unique_ptr<pqxx::work> txn = initialize_transaction();
            const pqxx::result response = txn->exec_params(query_string, params);
            finish_transaction(std::move(txn));
            return response;
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    pqxx::result PqxxClient::execute_query_with_result(const std::string& query_string) const
    {
        std::lock_guard lock(conn_mutex_);
        try
        {
            std::unique_ptr<pqxx::work> txn = initialize_transaction();
            const pqxx::result response = txn->exec(query_string);
            finish_transaction(std::move(txn));
            return response;
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    void PqxxClient::_oid_preprocess()
    {
        std::lock_guard lock(conn_mutex_);
        try
        {
            pqxx::nontransaction txn(*conn_);
            const pqxx::result r = txn.exec(
                "SELECT typname, oid FROM pg_type WHERE typname IN ('bool', 'int2', 'int4', 'int8', 'float4', 'float8', 'text', 'varchar', 'bpchar', 'timestamp', 'timestamptz')");
            for (const auto& row : r)
            {
                type_oids_.insert({row["oid"].as<uint32_t>(), row["typname"].c_str()});
            }
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    std::unique_ptr<FieldBase> PqxxClient::process_field(const pqxx::field& field) const
    {
        std::unique_ptr<FieldBase> field_ptr;

        // Get the field's PostgreSQL type OID
        boost::container::flat_map<unsigned, std::string>::const_iterator type_oid;
        {
            std::lock_guard lock(conn_mutex_);
            type_oid = type_oids_.find(field.type());
            if (type_oid == type_oids_.end())
            {
                throw std::invalid_argument("field type not found");
            }
        }

        // Determine the C++ type based on the PostgreSQL type OID and create the appropriate Field object
        if (type_oid->second == "int4") // 32-bit integer
        {
            field_ptr = std::make_unique<Field<int>>(field.name(), field.as<int32_t>());
            return field_ptr;
        }
        // Handle string types (text, varchar, char)
        if (type_oid->second == "text") // Text type
        {
            field_ptr = std::make_unique<Field<std::string>>(field.name(), field.as<std::string>());
            return field_ptr;
        }

        if (type_oid->second == "int8") // 64-bit integer (Bigint)
        {
            field_ptr = std::make_unique<Field<int64_t>>(field.name(), field.as<int64_t>());
            return field_ptr;
        }

        // Handle float types
        if (type_oid->second == "float8") // Double precision (64-bit float)
        {
            field_ptr = std::make_unique<Field<double>>(field.name(), field.as<double>());
            return field_ptr;
        }


        // Handle boolean type
        if (type_oid->second == "bool") // Boolean type
        {
            field_ptr = std::make_unique<Field<bool>>(field.name(), field.as<bool>());
            return field_ptr;
        }

        // Handle timestamp types (with or without timezone)
        if (type_oid->second == "timestamptz" || type_oid->second == "timestamp") // Timestamp type
        {
            const auto value = field.as<std::string>();
            // Parse the timestamp string into std::chrono::system_clock::time_point
            std::istringstream iss(value);
            std::tm tm = {};
            iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); // Parse the string to fill the tm structure
            auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm)); // Convert to time_point
            field_ptr = std::make_unique<Field<std::chrono::system_clock::time_point>>(field.name(), tp);
            return field_ptr;
        }

        // Return the field pointer (nullptr if no matching type was found)
        return field_ptr;
    }

    void PqxxClient::make_unique_constraint(const std::string_view table_name,
                                            std::vector<std::shared_ptr<FieldBase>> conflict_fields)
    {
        std::lock_guard lock(this->conn_mutex_);
        std::string tb = table_name.data();
        this->conflict_fields_[tb] = std::move(conflict_fields);
        auto conflict_fields_it = this->conflict_fields_.at(tb);
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        std::ostringstream sub_query;
        query_stream << "ALTER TABLE " << table << " ADD CONSTRAINT ";
        std::ostringstream name_constraint;
        sub_query << " UNIQUE (";
        for (const auto& column : conflict_fields_it)
        {
            sub_query << escape_identifier(column->get_name()) << ",";
            name_constraint << column->get_name() << "_";
        }
        name_constraint << table_name;
        std::string query = query_stream.str();
        query.append(escape_identifier(name_constraint.str())).append(sub_query.str());
        query.pop_back();
        query += ");";

        execute_query(query);
    }

    // Transaction Methods
    void PqxxClient::start_transaction()
    {
        std::lock_guard lock(this->conn_mutex_);
        if (in_transaction_)
        {
            throw TransactionException("Transaction already started.", db_err::TRANSACTION_START_FAILED);
        }
        try
        {
            open_transaction_ = std::make_unique<pqxx::work>(*conn_);
            in_transaction_ = true;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what(), db_err::QUERY_EXECUTION_FAILED);
        }
    }

    void PqxxClient::commit_transaction()
    {
        std::lock_guard lock(this->conn_mutex_);
        if (!in_transaction_)
        {
            throw TransactionException("No active transaction to commit.", db_err::TRANSACTION_COMMIT_FAILED);
        }
        try
        {
            open_transaction_->commit();
            open_transaction_.reset();
            in_transaction_ = false;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what(), db_err::QUERY_EXECUTION_FAILED);
        }
    }

    void PqxxClient::rollback_transaction()
    {
        std::lock_guard lock(this->conn_mutex_);
        if (!in_transaction_)
        {
            throw TransactionException("No active transaction to rollback.", db_err::TRANSACTION_ROLLBACK_FAILED);
        }
        try
        {
            open_transaction_->abort();
            open_transaction_.reset();
            in_transaction_ = false;
        }
        catch (const pqxx::sql_error& e)
        {
            throw TransactionException(e.what(), db_err::QUERY_EXECUTION_FAILED);
        }
    }


    // Table Management
    void PqxxClient::create_table(const std::string_view table_name, const Record& field_list)
    {
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "CREATE TABLE " << table << " (";

        for (const auto& field : field_list)
        {
            std::string name = escape_identifier(field->get_name());
            std::string type = field->get_sql_type();
            query_stream << name << " " << type << ", ";
        }

        std::string query = query_stream.str();
        query.erase(query.size() - 2); // Remove last comma and space
        query += ");";

        execute_query(query);
    }

    void PqxxClient::remove_table(const std::string_view table_name)
    {
        const std::string table = escape_identifier(table_name);
        const std::string query = "DROP TABLE IF EXISTS " + table + ";";
        execute_query(query);
    }

    bool PqxxClient::check_table(const std::string_view table_name)
    {
        const std::string table = conn_->esc(std::string(table_name));
        const std::string query = "SELECT EXISTS (SELECT 1 FROM information_schema.tables WHERE table_name = " + conn_->
            quote(table) + ");";
        const pqxx::result res = execute_query_with_result(query);
        return res[0][0].as<bool>();
    }

    // Data Manipulation Implementation
    void PqxxClient::insert_implementation(const std::string_view table_name, const std::vector<Record>& rows)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        execute_query(query, params);
    }

    void PqxxClient::insert_implementation(const std::string_view table_name, std::vector<Record>&& rows)
    {
        auto [query, params] = construct_insert_query(table_name, std::move(rows));
        execute_query(query, params);
    }

    // Upsert Data Implementation
    void PqxxClient::upsert_implementation(const std::string_view table_name,
                                           const std::vector<Record>& rows,
                                           const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        build_conflict_clause(query, table_name, replace_fields);
        execute_query(query, params);
    }

    void PqxxClient::upsert_implementation(const std::string_view table_name,
                                           std::vector<Record>&& rows,
                                           const std::vector<std::shared_ptr<FieldBase>>& replace_fields)
    {
        auto [query, params] = construct_insert_query(table_name, rows);
        build_conflict_clause(query, table_name, replace_fields);
        execute_query(query, params);
    }

    void PqxxClient::create_fts_index_query(const std::string_view table_name, std::ostringstream& index_query) const
    {
        const std::string table = escape_identifier(table_name);

        // Build the concatenated fields expression
        std::ostringstream fields_stream;
        for (const auto fts_fields = this->fts_fields_.at(table_name.data()); const auto& field : fts_fields)
        {
            fields_stream << "coalesce(" << escape_identifier(field->get_name()) << "::text, '') || ' ' || ";
        }
        std::string fields_concatenated = fields_stream.str();
        fields_concatenated.erase(fields_concatenated.size() - 11); // Remove last " || ' ' || "

        index_query << "CREATE INDEX IF NOT EXISTS " << make_fts_index_name(table_name) << " ON " << table
            << " USING gin (to_tsvector('simple', " << fields_concatenated << "));";
    }

    void PqxxClient::setup_full_text_search(
        const std::string_view table_name,
        std::vector<std::shared_ptr<FieldBase>> fields)
    {
        if (fields.empty())
        {
            throw QueryException("Invalid number of fts fields. Expected at least one element",
                                 db_err::INVALID_QUERY);
        }
        this->fts_fields_[table_name.data()] = std::move(fields);
        std::lock_guard lock(this->conn_mutex_);

        try
        {
            std::ostringstream index_query;
            create_fts_index_query(table_name, index_query);

            execute_query(index_query.str());
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    void PqxxClient::drop_full_text_search(const std::string_view table_name) const
    {
        std::lock_guard lock(this->conn_mutex_);
        try
        {
            std::ostringstream index_query;
            index_query << "DROP INDEX IF EXISTS " << make_fts_index_name(table_name) << ";";
            execute_query(index_query.str());
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    void PqxxClient::remove_full_text_search(const std::string_view table_name)
    {
        this->fts_fields_[std::string(table_name)].clear();
        drop_full_text_search(table_name);
    }

    void PqxxClient::restore_full_text_search(const std::string_view table_name) const
    {
        std::lock_guard lock(this->conn_mutex_);

        try
        {
            std::ostringstream index_query;
            create_fts_index_query(table_name, index_query);

            execute_query(index_query.str());
        }
        catch (const std::exception& e)
        {
            throw adapt_exception(e);
        }
    }

    // Data Retrieval
    std::vector<Record> PqxxClient::select(
        const std::string_view table_name,
        const FieldConditions& conditions) const
    {
        if (conditions.empty())
        {
            throw QueryException("Invalid number of conditions. For selecting all data call another function",
                                 db_err::INVALID_QUERY);
        }
        std::vector<Record> results;
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;

        pqxx::params params;
        query_stream << "SELECT * FROM " << table;
        query_stream << " WHERE ";
        uint32_t param_index = 1;
        for (const auto& condition : conditions)
        {
            std::string field_name = escape_identifier(condition.field()->get_name());
            query_stream << field_name << " " << condition.op() << " $" << param_index++ << " AND ";
            params.append(condition.value()->to_string());
        }
        std::string query = query_stream.str();
        query.erase(query.size() - 5); // Remove last ' AND '
        query += ";";
        const pqxx::result res = execute_query_with_result(query, params);
        results.reserve(res.size());
        for (const auto& row : res)
        {
            Record record;
            record.reserve(row.size());
            for (const auto& field : row)
            {
                record.push_back(process_field(field));
            }
            results.push_back(std::move(record));
        }
        return results;
    }

    std::vector<std::unique_ptr<ViewRecord>> PqxxClient::view(const std::string_view table_name,
                                                              const FieldConditions& conditions) const
    {
        if (conditions.empty())
        {
            throw QueryException("Invalid number of conditions. For selecting all data call another function",
                                 db_err::INVALID_QUERY);
        }
        std::vector<std::unique_ptr<ViewRecord>> results;
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;

        pqxx::params params;
        query_stream << "SELECT * FROM " << table;
        query_stream << " WHERE ";
        uint32_t param_index = 1;
        for (const auto& condition : conditions)
        {
            std::string field_name = escape_identifier(condition.field()->get_name());
            query_stream << field_name << " " << condition.op() << " $" << param_index++ << " AND ";
            params.append(condition.value()->to_string());
        }
        std::string query = query_stream.str();
        query.erase(query.size() - 5); // Remove last ' AND '
        query += ";";
        pqxx::result res = execute_query_with_result(query, params);
        results.reserve(res.size());
        for (auto&& row : std::move(res))
        {
            auto record = std::make_unique<PqxxViewRecord>();
            record->set_row(std::move(row));
            results.push_back(std::move(record));
        }
        return results;
    }

    std::vector<Record> PqxxClient::select(const std::string_view table_name) const
    {
        std::vector<Record> results;
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "SELECT * FROM " << table << ";";
        const pqxx::result res = execute_query_with_result(query_stream.str());
        results.reserve(res.size());
        for (const auto& row : res)
        {
            Record record;
            record.reserve(row.size());
            for (const auto& field : row)
            {
                record.push_back(process_field(field));
            }
            results.push_back(std::move(record));
        }
        return results;
    }

    std::vector<std::unique_ptr<ViewRecord>> PqxxClient::view(const std::string_view table_name) const
    {
        std::vector<std::unique_ptr<ViewRecord>> results;
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "SELECT * FROM " << table << ";";
        pqxx::result res = execute_query_with_result(query_stream.str());
        results.reserve(res.size());
        for (auto&& row : std::move(res))
        {
            auto record = std::make_unique<PqxxViewRecord>();
            record->set_row(std::move(row));
            results.push_back(std::move(record));
        }
        return results;
    }

    void PqxxClient::truncate_table(const std::string_view table_name)
    {
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "TRUNCATE TABLE " << table << ";";
        execute_query(query_stream.str());
    }

    void PqxxClient::remove(
        const std::string_view table_name,
        const FieldConditions& conditions)
    {
        if (conditions.empty())
        {
            throw QueryException("Invalid number of conditions. For removing all data call another function",
                                 db_err::INVALID_QUERY);
        }
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        pqxx::params params;
        query_stream << "DELETE FROM " << table;

        query_stream << " WHERE ";
        uint32_t param_index = 1;
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

    uint32_t PqxxClient::count(const std::string_view table_name) const
    {
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        query_stream << "SELECT COUNT(*) FROM " << table << ";";
        // Execute the query
        const pqxx::result res = execute_query_with_result(query_stream.str());

        return res[0][0].as<uint32_t>();
    }

    uint32_t PqxxClient::count(const std::string_view table_name,
                               const FieldConditions& conditions) const
    {
        if (conditions.empty())
        {
            throw QueryException("Invalid number of conditions. For count all data call another function",
                                 db_err::INVALID_QUERY);
        }
        const std::string table = escape_identifier(table_name);
        std::ostringstream query_stream;
        // Start building the query
        query_stream << "SELECT COUNT(*) FROM " << table;
        // Add conditions if any
        pqxx::params params;
        uint32_t param_index = 1;
        query_stream << " WHERE ";
        for (const auto& condition : conditions)
        {
            std::string field_name = escape_identifier(condition.field()->get_name());
            query_stream << field_name << " " << condition.op() << " $" << param_index++ << " AND ";
            params.append(condition.value()->to_string());
        }
        std::string query = query_stream.str();
        query.erase(query.size() - 5); // Remove last ' AND '
        query += ";";
        const pqxx::result res = execute_query_with_result(query, params);
        return res[0][0].as<uint32_t>();
    }


    // Full-Text Search Methods
    std::vector<Record> PqxxClient::get_data_fts(
        const std::string_view table_name,
        const std::string& fts_query_params) const
    {
        std::vector<Record> results;
        const std::string table = escape_identifier(table_name);

        // Build the concatenated fields expression
        std::ostringstream fields_stream;
        for (const auto fts_fields = fts_fields_.at(table_name.data()); const auto& field : fts_fields)
        {
            fields_stream << "coalesce(" << field->get_name() << "::text, '') || ' ' || ";
        }
        std::string fields_str = fields_stream.str();
        fields_str.erase(fields_str.size() - 11); // Remove last " || ' ' || "

        const std::string query = "SELECT * FROM " + table +
            " WHERE to_tsvector('simple', " + fields_str + ") @@ to_tsquery('simple', $1);";

        pqxx::params params;
        params.append(fts_query_params);

        for (const pqxx::result res = execute_query_with_result(query, params); const auto& row : res)
        {
            Record record;
            for (const auto& field : row)
            {
                record.push_back(process_field(field));
            }
            results.push_back(std::move(record));
        }
        return results;
    }

    std::string PqxxClient::make_fts_index_name(const std::string_view table_name)
    {
        std::ostringstream fts_ind;
        fts_ind << "fts_" << table_name << "_idx";
        return fts_ind.str();
    }
}
