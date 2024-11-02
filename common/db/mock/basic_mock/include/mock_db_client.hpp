// pqxx_client.hpp
#pragma once

#include <db_interface.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <regex>
#include <string_view>
#include <valarray>
#include <vector>


namespace drug_lib::common::database
{
    class MockDbClient final : public interfaces::DbInterface
    {
    public:
        /// @brief Creating database with given params using template db
        static void create_database(const std::string_view host,
                                    const uint32_t port,
                                    const std::string_view db_name,
                                    const std::string_view login,
                                    [[maybe_unused]] std::string_view password)
        {
            std::cout << "Creating database " << host << ":" << port << "\nDatabase name: " << db_name <<
                "\nCredentials: " << login << " " << "***********" << std::endl;
        }

        /// @brief Explicitly close the connection. Destructor do the same, but this function can throw exception
        void drop_connect() override
        {
            std::cout << "drop_connect " << std::endl;
        }


        /// @brief Trying to connect to the database, if connection is not open will throw exception
        /// @throws drug_lib::common::database::exceptions::ConnectionException
        MockDbClient(const std::string_view host,
                     const uint32_t port,
                     const std::string_view db_name,
                     const std::string_view login,
                     [[maybe_unused]] std::string_view password)
        {
            std::cout << "Connected to the database " << host << ":" << port << "\nDatabase name: " << db_name <<
                "\nCredentials: " << login << " " << "***********" << std::endl;
        }

        MockDbClient() = default;
        ~MockDbClient() override = default;

        // Transaction Methods
        /// @brief Start a transaction. All queries before commiting/roll backing a transaction will use common transaction
        /// @throws  drug_lib::common::database::exceptions::TransactionException
        void start_transaction() override
        {
            std::cout << "start_transaction " << std::endl;
        }

        /// @brief Commit a transaction. If throws an exception, all transaction will be reverted
        /// @throws  drug_lib::common::database::exceptions::TransactionException
        void commit_transaction() override
        {
            std::cout << "commit_transaction " << std::endl;
        }

        /// @brief Instantly cancel a transaction
        /// @throws  drug_lib::common::database::exceptions::TransactionException
        void rollback_transaction() override
        {
            std::cout << "rollback_transaction " << std::endl;
        }

        /// @brief Create unique index for the table
        /// @param table_name For which table created index.
        /// @param conflict_fields Fields which will be unique for each record
        void make_unique_constraint(std::string_view table_name,
                                    std::vector<std::shared_ptr<FieldBase>> conflict_fields) override
        {
            std::cout << "make_unique_constraint " << std::endl;
        }

        /// @brief Create full test search index for given fields
        /// @param table_name For which table created index.
        /// @param fields Fts fields
        void setup_fts_index(
            std::string_view table_name,
            std::vector<std::shared_ptr<FieldBase>> fields) override
        {
            std::cout << "setup_full_text_search " << std::endl;
        }

        /// @brief Drop index, but doesn't remove fts fields from this client. Allows to restore it(reindex) using restore_full_text_search method
        void drop_fts_index(std::string_view table_name) const override
        {
            std::cout << "drop_full_text_search " << std::endl;
        }

        /// @brief Drop index + remove fields from this client. For using fts further setup_fulltext_search should be called again
        void remove_fts_index(std::string_view table_name) override
        {
            std::cout << "remove_full_text_search " << std::endl;
        }

        /// @brief Restore index + reindex. Use previous declared fts fields
        void restore_fts_index(std::string_view table_name) const override
        {
            std::cout << "restore_full_text_search " << std::endl;
        }

        // Table Management

        /// @param table_name new table name
        /// @param field_list properties of fields of new table
        void create_table(std::string_view table_name, const Record& field_list) override
        {
            std::cout << "create_table " << std::endl;
        }

        void remove_table(std::string_view table_name) override
        {
            std::cout << "remove_table " << std::endl;
        }

        void truncate_table(std::string_view table_name) override
        {
            std::cout << "truncate_table " << std::endl;
        }

        /// @return Existence status of table
        [[nodiscard]] bool check_table(std::string_view table_name) override
        {
            std::cout << "check_table " << std::endl;
            return true;
        }

        // Data Retrieval

        /// @brief Select records follow conditions. Copy all data to own it
        /// @return Vector of records. Each element of vector - one row in a table
        [[nodiscard]] std::vector<Record> select(
            std::string_view table_name,
            const Conditions& conditions) const override
        {
            std::cout << "select" << std::endl;
            return {};
        }

        /// @brief Select all table records.
        /// @warning Slow, because of copying all fields
        [[nodiscard]] std::vector<Record> select(
            std::string_view table_name) const override
        {
            std::cout << "select" << std::endl;
            return {};
        }

        /// @brief Faster than select, but doesn't transform and allows only one operation view field
        /// @return Vector of view records. Each element of vector - one row in a table
        /// @warning If u need not only view data, use select.
        [[nodiscard]] std::vector<std::unique_ptr<ViewRecord>> view(
            std::string_view table_name,
            const Conditions& conditions) const override
        {
            std::cout << "view" << std::endl;
            return {};
        }

        /// @brief Faster than select, but doesn't transform and allows only one operation view field
        /// @return Vector of view records. Each element of vector - one row in a table
        /// @warning If u need not only view data, use select.
        [[nodiscard]] std::vector<std::unique_ptr<ViewRecord>> view(std::string_view table_name) const override
        {
            std::cout << "view" << std::endl;
            return {};
        }

        // Remove Data
        ///@brief remove data following conditions
        void remove(std::string_view table_name,
                    const Conditions& conditions) override
        {
            std::cout << "remove " << std::endl;
        }

        // Get Record Count
        /// @return Count of records
        [[nodiscard]] uint32_t count(std::string_view table_name,
                                     const Conditions& conditions) const override
        {
            std::cout << "count" << std::endl;
            return 0;
        }

        /// @return Count of all records in table
        [[nodiscard]] uint32_t count(std::string_view table_name) const override
        {
            std::cout << "count" << std::endl;
            return 0;
        }

    protected:
        // Implementation Methods for Data Manipulation
        void insert_implementation(std::string_view table_name, const std::vector<Record>& rows) override
        {
            std::cout << "insert_implementation " << std::endl;
        }

        void insert_implementation(std::string_view table_name, std::vector<Record>&& rows) override
        {
            std::cout << "insert_implementation " << std::endl;
        }

        void upsert_implementation(std::string_view table_name,
                                   const std::vector<Record>& rows,
                                   const std::vector<std::shared_ptr<FieldBase>>& replace_fields) override
        {
            std::cout << "upsert_implementation " << std::endl;
        }

        void upsert_implementation(std::string_view table_name,
                                   std::vector<Record>&& rows,
                                   const std::vector<std::shared_ptr<FieldBase>>& replace_fields) override
        {
            std::cout << "upsert_implementation " << std::endl;
        }
    };
}
