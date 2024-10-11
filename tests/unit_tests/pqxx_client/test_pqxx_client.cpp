// pqxx_client_test.cpp

#include <chrono>
#include <functional>
#include <gtest/gtest.h>

#include "db_conditions.hpp"
#include "db_field.hpp"
#include "db_record.hpp"
#include "pqxx_client.hpp"

using namespace drug_lib::common::database;

class PqxxClientTest : public testing::Test
{
protected:
    // Database connection parameters
    std::string host = "localhost";
    int port = 5432;
    std::string db_name = "test_db";
    std::string username = "postgres";
    std::string password = "postgres"; // Replace with your actual password

    // Pointer to the PqxxClient
    std::unique_ptr<PqxxClient> db_client;

    // Test table name
    std::string test_table = "test_table";

    // SetUp and TearDown methods
    void SetUp() override
    {
        // Initialize the database client
        db_client = std::make_unique<PqxxClient>(host, port, db_name, username, password);

        // Ensure the test table does not exist before starting
        if (db_client->check_table(test_table))
        {
            db_client->remove_table(test_table);
        }

        // Create a test table
        Record fields;
        fields.add_field(std::make_shared<Field<int>>("id", 0));
        fields.add_field(std::make_shared<Field<std::string>>("name", ""));
        fields.add_field(std::make_shared<Field<std::string>>("description", ""));
        db_client->create_table(test_table, fields);
        db_client->make_unique_constraint(test_table, {std::make_shared<Field<int>>("id", 0)});
        // Set up full-text search on 'name' and 'description' fields
        std::vector<std::shared_ptr<FieldBase>> fts_fields = {
            std::make_shared<Field<std::string>>("name", ""),
            std::make_shared<Field<std::string>>("description", "")
        };

        db_client->setup_full_text_search(test_table, std::move(fts_fields));
    }

    void TearDown() override
    {
        // Remove the test table after each test
        if (db_client->check_table(test_table))
        {
            db_client->remove_table(test_table);
        }

        db_client.reset();
    }
};


TEST_F(PqxxClientTest, TransactionTest)
{
    EXPECT_NO_THROW(db_client->start_transaction());
    EXPECT_NO_THROW(db_client->commit_transaction());

    // Test rollback
    EXPECT_NO_THROW(db_client->start_transaction());
    EXPECT_NO_THROW(db_client->rollback_transaction());
}

TEST_F(PqxxClientTest, TableManagementTest)
{
    // Check if the table exists
    EXPECT_TRUE(db_client->check_table(test_table));

    // Remove the table
    EXPECT_NO_THROW(db_client->remove_table(test_table));
    EXPECT_FALSE(db_client->check_table(test_table));

    // Create the table again
    Record fields;
    fields.add_field(std::make_shared<Field<int>>("id", 0));
    fields.add_field(std::make_shared<Field<std::string>>("name", ""));
    fields.add_field(std::make_shared<Field<std::string>>("description", ""));

    EXPECT_NO_THROW(db_client->create_table(test_table, fields));
    EXPECT_TRUE(db_client->check_table(test_table));
}

TEST_F(PqxxClientTest, AddDataTest)
{
    // Create sample data
    std::vector<Record> records;

    Record record1;
    record1.add_field(std::make_shared<Field<int>>("id", 1));
    record1.add_field(std::make_shared<Field<std::string>>("name", "Alice"));
    record1.add_field(std::make_shared<Field<std::string>>("description", "P"));
    records.push_back(std::move(record1));

    Record record2;
    record2.add_field(std::make_shared<Field<int>>("id", 2));
    record2.add_field(std::make_shared<Field<std::string>>("name", "Bob"));
    record2.add_field(std::make_shared<Field<std::string>>("description", "L"));
    records.push_back(std::move(record2));

    // Add data to the table
    EXPECT_NO_THROW(db_client->add_data(test_table, records));

    // Retrieve data and verify
    const FieldConditions conditions;
    const auto results = db_client->get_data(test_table, conditions);

    EXPECT_EQ(results.size(), 2);

    // Check content
    for (const auto& rec : results)
    {
        const auto id = rec.at("id")->as<int32_t>();
        const auto name = rec.at("name")->as<std::string>();

        if (id == 1)
        {
            EXPECT_EQ(name, "Alice");
        }
        else if (id == 2)
        {
            EXPECT_EQ(name, "Bob");
        }
        else
        {
            FAIL() << "Unexpected record with id: " << id;
        }
    }
}

TEST_F(PqxxClientTest, UpsertDataTest)
{
    // Initial data
    std::vector<Record> records;

    Record record1;
    record1.add_field(std::make_shared<Field<int>>("id", 1));
    record1.add_field(std::make_shared<Field<std::string>>("name", "Alice"));
    record1.add_field(std::make_shared<Field<std::string>>("description", ""));
    records.push_back(std::move(record1));

    // Add initial data
    EXPECT_NO_THROW(db_client->add_data(test_table, records));

    // Upsert data
    std::vector<Record> upsert_records;

    Record record_upsert;
    record_upsert.add_field(std::make_shared<Field<int>>("id", 1));
    record_upsert.add_field(std::make_shared<Field<std::string>>("name", "Alice Updated"));
    record_upsert.add_field(std::make_shared<Field<std::string>>("description", "XXX"));
    upsert_records.push_back(std::move(record_upsert));

    // Conflict and replace fields
    const std::vector<std::shared_ptr<FieldBase>> replace_fields = {
        std::make_shared<Field<std::string>>("name", ""),
        std::make_shared<Field<std::string>>("description", "")
    };

    EXPECT_NO_THROW(db_client->upsert_data(test_table, upsert_records, replace_fields));

    // Retrieve data and verify
    const FieldConditions conditions;
    const auto results = db_client->get_data(test_table, conditions);

    EXPECT_EQ(results.size(), 1);

    const auto& rec = results.front();
    const auto id = rec.at("id")->as<int32_t>();
    const auto name = rec.at("name")->as<std::string>();
    const auto description = rec.at("description")->as<std::string>();
    EXPECT_EQ(id, 1);
    EXPECT_EQ(name, "Alice Updated");
    EXPECT_EQ(description, "XXX");
}

TEST_F(PqxxClientTest, RemoveDataTest)
{
    // Add data
    std::vector<Record> records;

    Record record1;
    record1.add_field(std::make_shared<Field<int>>("id", 1));
    record1.add_field(std::make_shared<Field<std::string>>("name", "Alice"));
    record1.add_field(std::make_shared<Field<std::string>>("description", ""));
    records.push_back(std::move(record1));

    EXPECT_NO_THROW(db_client->add_data(test_table, records));

    // Remove data
    FieldConditions conditions;
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int>>("id", 0),
        "=",
        std::make_unique<Field<int>>("", 1)
    ));

    EXPECT_NO_THROW(db_client->remove_data(test_table, conditions));

    // Verify removal
    const auto results = db_client->get_data(test_table, FieldConditions());
    EXPECT_TRUE(results.empty());
}

TEST_F(PqxxClientTest, GetCountTest)
{
    // Add data
    std::vector<Record> records;

    for (int i = 1; i <= 5; ++i)
    {
        Record record;
        record.add_field(std::make_shared<Field<int>>("id", i));
        record.add_field(std::make_shared<Field<std::string>>("name", "User" + std::to_string(i)));
        record.add_field(std::make_shared<Field<std::string>>("description", ""));
        records.push_back(std::move(record));
    }

    EXPECT_NO_THROW(db_client->add_data(test_table, records));

    // Get count
    std::chrono::duration<double> query_time{};
    FieldConditions conditions;
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int>>("id", 0),
        "=",
        std::make_unique<Field<int>>("", 1)
    ));
    const int count = db_client->count(test_table, conditions, query_time);

    EXPECT_EQ(count, 1);
    const int count_all = db_client->count(test_table, {}, query_time);
    EXPECT_EQ(count_all, 5);
}

TEST_F(PqxxClientTest, FullTextSearchTest)
{
    // Add data to the table
    std::vector<Record> records;

    Record record1;
    record1.add_field(std::make_shared<Field<int>>("id", 1));
    record1.add_field(std::make_shared<Field<std::string>>("name", "Apple"));
    record1.add_field(std::make_shared<Field<std::string>>("description", "A sweet red fruit"));
    records.push_back(std::move(record1));

    Record record2;
    record2.add_field(std::make_shared<Field<int>>("id", 2));
    record2.add_field(std::make_shared<Field<std::string>>("name", "Banana"));
    record2.add_field(std::make_shared<Field<std::string>>("description", "A long yellow fruit"));
    records.push_back(std::move(record2));

    Record record3;
    record3.add_field(std::make_shared<Field<int>>("id", 3));
    record3.add_field(std::make_shared<Field<std::string>>("name", "Carrot"));
    record3.add_field(std::make_shared<Field<std::string>>("description", "An orange root vegetable"));
    records.push_back(std::move(record3));

    // Insert records into the database
    EXPECT_NO_THROW(db_client->add_data(test_table, records));

    // Perform full-text search
    std::chrono::duration<double> query_time{};
    std::string search_query = "fruit";
    auto results = db_client->get_data_fts(test_table, search_query, query_time);

    // Verify the results
    EXPECT_EQ(results.size(), 2);

    std::set<int> expected_ids = {1, 2};
    std::set<int> result_ids;
    for (const auto& rec : results)
    {
        int id = rec.at("id")->as<int32_t>();
        result_ids.insert(id);
    }

    EXPECT_EQ(result_ids, expected_ids);

    // Search for 'yellow'
    search_query = "yellow";
    results = db_client->get_data_fts(test_table, search_query, query_time);

    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.front().at("name")->as<std::string>(), "Banana");

    // Search for 'vegetable'
    search_query = "vegetable";
    results = db_client->get_data_fts(test_table, search_query, query_time);

    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.front().at("name")->as<std::string>(), "Carrot");

    // Search for a term not present
    search_query = "berry";
    results = db_client->get_data_fts(test_table, search_query, query_time);

    EXPECT_TRUE(results.empty());
}


int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
