#pragma once

#include "db_interface.hpp"

namespace drug_lib::common::db
{
    struct PostgresConnectionParams
    {
        std::string host;
        int port;
        std::string dbName;
        std::string login;
        std::string password;

        // Constructor
        PostgresConnectionParams(std::string host, int port, std::string dbName, std::string login, std::string password)
            : host(std::move(host)), port(port), dbName(std::move(dbName)), login(std::move(login)), password(std::move(password)) {}
    };

    class PqxxClient : public interfaces::DBInterface
    {
      /**
     * @brief PostgreSQL implementation of the IStationDatabase interface.
     */
    class StationDatabasePostgres : public IStationDatabase
    {
    public:
        StationDatabasePostgres(const std::string &host,
                                int port,
                                const std::string &dbName,
                                const std::string &login,
                                const std::string &password)
            : inTransaction_(false)
        {
            try
            {

                std::string connectionStr = "host=" + host + " port=" + std::to_string(port) +
                                            " user=" + login + " password=" + password + " dbname=template1";

                // Establish connection to the 'template1' database
                pqxx::connection C(connectionStr);

                if (C.is_open())
                {

                    // Start a non-transactional query to create the database
                    pqxx::nontransaction N(C);

                    // Prepare the parameterized SQL query
                    std::string query = "CREATE DATABASE " + dbName +
                                        R"( WITH OWNER = postgres
                    ENCODING = 'UTF8'
                    TABLESPACE = pg_default
                    CONNECTION LIMIT = -1
                    IS_TEMPLATE = False;)";

                    // Execute the query with the provided database name as a parameter
                    N.exec(query);

                    std::cout << "Database " << dbName << " created successfully!" << std::endl;

                    // Close the connection
                    C.close();
                    conn_ = std::make_shared<pqxx::connection>(
                        "dbname=" + dbName +
                        " user=" + login +
                        " password=" + password +
                        " host=" + host +
                        " port=" + std::to_string(port));
                }
                else
                {
                    std::cerr << "Connection to the database failed." << std::endl;
                    return;
                }
            }
            catch (const pqxx::sql_error &e)
            {
            }
            conn_ = std::make_shared<pqxx::connection>(
                "dbname=" + dbName +
                " user=" + login +
                " password=" + password +
                " host=" + host +
                " port=" + std::to_string(port));
            strategy.Batcher.SetAction(std::bind(&ControlSystemV7::Common::Database::StationDatabasePostgres::DBExecute, this, std::placeholders::_1, std::placeholders::_2));
            strategy.Batcher.SetSupport(false);
            strategy.Batcher.Disable();
        }

        void StartTransaction() override;
        void CommitTransaction() override;
        void RollbackTransaction() override;

        void CreateTable(const std::string &tableName, const std::vector<FieldDescription> &fieldList) override;

        void RemoveTable(const std::string &tableName) override;

        bool HasTable(const std::string &tableName) const override;

        void MakeUniqueConstraint(const std::string &tableName, std::vector<std::string> &&columnsnames) override;

        void RemoveData(const std::string &tableName, std::vector<std::pair<std::string, std::string>> &&keyValues) override;

        std::vector<std::vector<FieldDescription>> GetData(const std::string &tableName, std::vector<std::pair<std::string, std::string>> &&keyValues) override;

        std::vector<std::vector<FieldDescription>> GetDataFTS(const std::string &tableName, const std::string &ftsQueryParams, double &queryExecTime) override;

        bool GetDataFTSBatched(const std::string &tableName,
                               const std::string &ftsQueryParams,
                               double &queryExecTime,
                               std::function<void(const std::vector<std::vector<FieldDescription>> &)> onResult) override;

        int GetCount(const std::string &tableName, const std::string &fieldName, double &queryExecTime) override;

        void AddData(const std::string &tableName, const std::vector<std::vector<FieldDescription>> &rows) override
        {
            AddDataInternal(tableName, rows); // lvalue version
        }

        void AddData(const std::string &tableName, std::vector<std::vector<FieldDescription>> &&rows) override
        {
            AddDataInternal(tableName, rows); // rvalue version
        }

        void UpsertData(const std::string &tableName,
                        const std::vector<std::vector<FieldDescription>> &rows,
                        std::vector<std::string> conflictFields,
                        std::vector<std::string> replaceFields) override
        {
            UpsertDataInternal(tableName, rows, std::move(conflictFields), std::move(replaceFields)); // lvalue version
        }

        void UpsertData(const std::string &tableName,
                        std::vector<std::vector<FieldDescription>> &&rows,
                        std::vector<std::string> conflictFields,
                        std::vector<std::string> replaceFields) override
        {
            UpsertDataInternal(tableName, std::move(rows), std::move(conflictFields), std::move(replaceFields)); // rvalue version
        }

    private:
        Strategies::StrategyControl<std::string, pqxx::params> strategy;
        std::shared_ptr<pqxx::connection> conn_;
        std::unique_ptr<pqxx::work> txn_;
        bool inTransaction_;

        int databaseVersion = 1; // Hardcoded in the database implementation

        bool IsValidIdentifier(const std::string &identifier) const;

        std::string EscapeJsonKeys(const std::string &jsonStr);
        std::string EscapeJsonKeys(std::string &&jsonStr);

        void DBExecute(std::string queryString, pqxx::params paramValues)
        {
            pqxx::work txn(*conn_);
            txn.exec_params(queryString, paramValues);
            txn.commit();
        }

#include <sstream>

        template <typename T, typename = std::enable_if_t<
                                  std::is_same<std::decay_t<T>, std::vector<std::vector<FieldDescription>>>::value>>
        std::pair<std::string, pqxx::params> ConstructRows(T &&rows)
        {
            if (rows.empty())
            {
                return {"", {}};
            }

            std::ostringstream queryStream;
            std::ostringstream namesStream;
            pqxx::params paramValues;
            int paramCounter = 1;
            bool namesSet = false;

            for (const auto &fieldList : rows)
            {
                std::ostringstream rowValues;
                rowValues << "(";

                for (const auto &field : fieldList)
                {
                    if (!namesSet)
                    {
                        namesStream << field.name << ", ";
                    }

                    rowValues << "$" << paramCounter++ << ", ";
                    paramValues.append(field.value);
                }

                rowValues.seekp(-2, std::ios_base::end);
                rowValues << "), ";
                queryStream << rowValues.str();
                namesSet = true;
            }

            std::string names = namesStream.str();
            names.erase(names.size() - 2); // Remove trailing comma and space

            std::string values = queryStream.str();
            values.erase(values.size() - 2); // Remove trailing comma and space

            std::string queryString = "(" + names + ") VALUES " + values;

            return {std::move(queryString), std::move(paramValues)};
        }

        template <typename T, typename = std::enable_if_t<
                                  std::is_same<std::decay_t<T>, std::vector<std::vector<ControlSystemV7::Common::Database::FieldDescription>>>::value>>
        void AddDataInternal(const std::string &tableName, T &&rows)
        {
            if (rows.empty())
                return;

            if (!IsValidIdentifier(tableName))
            {
                throw Exceptions::AddRowException("Invalid table name: " + tableName);
            }

            try
            {
                // Construct the combined query and parameters using perfect forwarding
                auto [queryString, paramValuesVector] = ConstructRows(std::forward<T>(rows));

                // Prepend the INSERT INTO part
                queryString = "INSERT INTO " + tableName + " " + queryString + ";";

                if (strategy.Batcher.IsEnabled())
                {
                    strategy.Batcher.execute(std::move(queryString), std::move(paramValuesVector));
                }
                else
                {
                    DBExecute(std::move(queryString), std::move(paramValuesVector));
                }
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << e.what() << std::endl;
                throw;
            }

            return;
        }

        template <typename T, typename = std::enable_if_t<
                                  std::is_same<std::decay_t<T>, std::vector<std::vector<ControlSystemV7::Common::Database::FieldDescription>>>::value>>
        void UpsertDataInternal(const std::string &tableName,
                                T &&rows,
                                std::vector<std::string> &&conflictFields,
                                std::vector<std::string> &&replaceFields)
        {
            if (!IsValidIdentifier(tableName))
            {
                throw Exceptions::AddRowException("Invalid table name: " + tableName);
            }

            try
            {
                // Construct the base query
                std::string queryString = "INSERT INTO " + tableName + " ";

                // Construct the combined rows query and parameters using perfect forwarding
                auto [rowsQuery, paramValuesVector] = ConstructRows(std::forward<T>(rows));
                queryString.append(rowsQuery);
                queryString.append(" ON CONFLICT (");

                // Handle conflict fields
                for (auto &&field : conflictFields)
                {
                    if (!IsValidIdentifier(field))
                    {
                        throw Exceptions::AddRowException("Invalid conflict field: " + field);
                    }
                    queryString.append(std::move(field) + ",");
                }
                queryString.pop_back(); // Remove the trailing comma
                queryString.append(") DO UPDATE SET");

                // Handle replace fields
                for (auto &&field : replaceFields)
                {
                    if (!IsValidIdentifier(field))
                    {
                        throw Exceptions::AddRowException("Invalid replace field: " + field);
                    }
                    queryString.append(" " + field + " = EXCLUDED." + std::move(field) + ",");
                }
                queryString.pop_back(); // Remove the trailing comma
                queryString.append(";");
                if (strategy.Batcher.IsEnabled())
                {
                    strategy.Batcher.execute(std::move(queryString), std::move(paramValuesVector));
                }
                else
                {
                    DBExecute(std::move(queryString), std::move(paramValuesVector));
                }
            }
            catch (const std::runtime_error &e)
            {
                std::cerr << e.what() << std::endl;
                throw;
            }
        }
    }; 
    };
}