#include <memory>
#include <gtest/gtest.h>

#include "db_interface_factory.hpp"
#include "transaction_manager.hpp"

class TransactionManagerTest : public testing::Test
{
protected:
    // The TransactionManager instance to be tested
    drug_lib::common::db::interfaces::TransactionManager transaction_manager;

    // Test table names
    std::string table1 = "table1";
    std::string table2 = "table2";

    // Shared pointers for mock database connections and mutexes
    std::shared_ptr<drug_lib::common::database::interfaces::DbInterface> mock_db1;
    std::shared_ptr<drug_lib::common::database::interfaces::DbInterface> mock_db2;
    std::shared_ptr<std::recursive_mutex> mutex1;
    std::shared_ptr<std::recursive_mutex> mutex2;

    void SetUp() override
    {
        // Initialize mocks and mutexes
        mock_db1 = drug_lib::common::database::creational::DbInterfaceFactory::create_mock_database();
        mock_db2 = drug_lib::common::database::creational::DbInterfaceFactory::create_mock_database(
            "123.123.123.123", 23, "mock_db1.db", "0.0.0.0", "123133");
        mutex1 = std::make_shared<std::recursive_mutex>();
        mutex2 = std::make_shared<std::recursive_mutex>();

        // Add tables to TransactionManager
        transaction_manager.add_table(table1, mock_db1, mutex1);
        transaction_manager.add_table(table2, mock_db2, mutex2);
    }
};

TEST_F(TransactionManagerTest, AddTable)
{
    // Verify that tables have been added
    EXPECT_NO_THROW(transaction_manager.start_transaction(table1));
    EXPECT_NO_THROW(transaction_manager.start_transaction(table2));
}

TEST_F(TransactionManagerTest, RemoveTable)
{
    // Remove table1 and verify it has been removed
    transaction_manager.remove_table(table1);
    EXPECT_THROW(transaction_manager.start_transaction(table1), std::runtime_error);
}

TEST_F(TransactionManagerTest, StartTransaction)
{
    // Start a transaction on table1
    EXPECT_NO_THROW(transaction_manager.start_transaction(table1));
}

TEST_F(TransactionManagerTest, CommitTransaction)
{
    EXPECT_NO_THROW(transaction_manager.start_transaction(table1));
    EXPECT_NO_THROW(transaction_manager.commit_transaction(table1));
}

TEST_F(TransactionManagerTest, RollbackTransaction)
{
    EXPECT_NO_THROW(transaction_manager.start_transaction(table1));
    EXPECT_NO_THROW(transaction_manager.rollback_transaction(table1));
}

TEST_F(TransactionManagerTest, TransactionAlreadyInProgress)
{
    EXPECT_NO_THROW(transaction_manager.start_transaction(table1));
    EXPECT_THROW(transaction_manager.start_transaction(table1), std::runtime_error);
}
