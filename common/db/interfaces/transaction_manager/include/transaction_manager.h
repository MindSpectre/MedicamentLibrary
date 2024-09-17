#pragma once
#include <mutex>
#include <memory>
#include <vector>
#include <tuple>
#include <functional>
#include <json/json.h>          // Include the JsonCpp library
// #include "IStationDatabase.hpp" // Your IStationDatabase interface
// #include "StationDatabaseFactory.hpp"
#include "db/interfaces/exceptions/exceptions.hpp"
#include <boost/algorithm/string.hpp>

namespace ControlSystemV7::Common::Services
{
    class TransactionManager
    {
    public:
        // Default Constructor
        TransactionManager() = default;

        // Move Constructor and Move Assignment Operator
        TransactionManager(TransactionManager &&other) noexcept
            : tableConnections(std::move(other.tableConnections)),
              tableMutexes(std::move(other.tableMutexes)),
              tableTransactionStates(std::move(other.tableTransactionStates)) {}

        TransactionManager &operator=(TransactionManager &&other) noexcept
        {
            if (this != &other)
            {
                std::lock_guard<std::mutex> lock(internalMutex);
                tableConnections = std::move(other.tableConnections);
                tableMutexes = std::move(other.tableMutexes);
                tableTransactionStates = std::move(other.tableTransactionStates);
            }
            return *this;
        }

        // Adds a new table with its associated database connection and mutex
        void AddTable(const std::string &tableName,
                      std::shared_ptr<ControlSystemV7::Common::Database::IStationDatabase> dbConnection,
                      std::shared_ptr<std::recursive_mutex > tableMutex)
        {
            std::lock_guard<std::mutex> lock(internalMutex);
            tableConnections[tableName] = std::move(dbConnection);
            tableMutexes[tableName] = std::move(tableMutex);
            tableTransactionStates[tableName] = false;
            std::cout << "Table " << tableName << " added with its own database connection and associated mutex." << std::endl;
        }

        // Removes a table and its associated connection and mutex
        void RemoveTable(const std::string &tableName)
        {
            std::lock_guard<std::mutex> lock(internalMutex);
            tableConnections.erase(tableName);
            tableMutexes.erase(tableName);
            tableTransactionStates.erase(tableName);
            std::cout << "Table " << tableName << " removed." << std::endl;
        }

        // Starts transactions on multiple tables
        template <typename... Args>
        void StartTransaction(Args... tableNames)
        {
            std::lock_guard<std::mutex> lock(internalMutex);
            (startTransaction(tableNames), ...);
            std::cout << "Transactions started on multiple tables." << std::endl;
        }

        // Commits transactions on multiple tables
        template <typename... Args>
        void CommitTransaction(Args... tableNames)
        {
            std::lock_guard<std::mutex> lock(internalMutex);
            (commitTransaction(tableNames), ...);
            std::cout << "Transactions committed on multiple tables." << std::endl;
        }

        // Rolls back transactions on multiple tables
        template <typename... Args>
        void RollbackTransaction(Args... tableNames)
        {
            std::lock_guard<std::mutex> lock(internalMutex);
            (rollbackTransaction(tableNames), ...);
            std::cout << "Transactions rolled back on multiple tables." << std::endl;
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<ControlSystemV7::Common::Database::IStationDatabase>> tableConnections;
        std::unordered_map<std::string, std::shared_ptr<std::recursive_mutex >> tableMutexes;
        std::unordered_map<std::string, bool> tableTransactionStates; // Tracks transaction state per table
        std::mutex internalMutex;                                     // Protects the access to the maps

        // Starts a transaction on a specific table
        void startTransaction(const std::string &tableName)
        {
            checkAndStartTableTransaction(tableName);
            lockTable(tableName);
            std::cout << "Transaction started on table " << tableName << "." << std::endl;
        }

        // Commits a transaction on a specific table
        void commitTransaction(const std::string &tableName)
        {
            checkTableTransactionInProgress(tableName);
            unlockTable(tableName);
            tableConnections[tableName]->CommitTransaction();
            tableTransactionStates[tableName] = false;
            std::cout << "Transaction committed on table " << tableName << "." << std::endl;
        }

        // Rolls back a transaction on a specific table
        void rollbackTransaction(const std::string &tableName)
        {
            checkTableTransactionInProgress(tableName);
            unlockTable(tableName);
            tableConnections[tableName]->RollbackTransaction();
            tableTransactionStates[tableName] = false;
            std::cout << "Transaction rolled back on table " << tableName << "." << std::endl;
        }

        // Locks the table-specific mutex
        void lockTable(const std::string &tableName)
        {
            auto tableMutex = getTableMutex(tableName);
            if (tableMutex)
            {
                tableMutex->lock();
            }
        }

        // Unlocks the table-specific mutex
        void unlockTable(const std::string &tableName)
        {
            auto tableMutex = getTableMutex(tableName);
            if (tableMutex)
            {
                tableMutex->unlock();
            }
        }

        // Gets the mutex for a specific table
        std::shared_ptr<std::recursive_mutex > getTableMutex(const std::string &tableName)
        {
            auto it = tableMutexes.find(tableName);
            if (it != tableMutexes.end())
            {
                return it->second;
            }
            return nullptr;
        }

        // Starts a transaction for a specific table, checking if already in progress
        void checkAndStartTableTransaction(const std::string &tableName)
        {
            if (tableTransactionStates[tableName])
            {
                throw std::runtime_error("Transaction is already in progress for table: " + tableName);
            }
            tableConnections[tableName]->StartTransaction();
            tableTransactionStates[tableName] = true;
        }

        // Checks if a transaction is in progress for a specific table
        void checkTableTransactionInProgress(const std::string &tableName)
        {
            if (!tableTransactionStates[tableName])
            {
                throw std::runtime_error("No transaction in progress for table: " + tableName);
            }
        }
    };

}
