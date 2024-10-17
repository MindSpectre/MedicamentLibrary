// pqxx_client_test.cpp

#include <chrono>
#include <functional>
#include <gtest/gtest.h>

#include "db_conditions.hpp"
#include "db_field.hpp"
#include "db_record.hpp"
#include "pqxx_client.hpp"
#include "stopwatch.hpp"
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
        fields.push_back(std::make_unique<Field<int>>("id", 0));
        fields.push_back(std::make_unique<Field<std::string>>("name", ""));
        fields.push_back(std::make_unique<Field<std::string>>("description", ""));
        db_client->create_table(test_table, fields);

        db_client->make_unique_constraint(test_table, {std::make_shared<Field<int>>("id", 0)});
        // Set up full-text search on 'name' and 'description' fields
        std::vector<std::shared_ptr<FieldBase>> fts_fields;
        fts_fields.emplace_back(std::make_shared<Field<std::string>>("name", ""));
        fts_fields.emplace_back(std::make_shared<Field<std::string>>("description", ""));


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
    fields.push_back(std::make_unique<Field<int>>("id", 0));
    fields.push_back(std::make_unique<Field<std::string>>("name", ""));
    fields.push_back(std::make_unique<Field<std::string>>("description", ""));

    EXPECT_NO_THROW(db_client->create_table(test_table, fields));
    EXPECT_TRUE(db_client->check_table(test_table));
}

TEST_F(PqxxClientTest, InsertTest)
{
    // Create sample data
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int>>("id", 1));
    record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record1.push_back(std::make_unique<Field<std::string>>("description", "P"));
    records.push_back(std::move(record1));

    Record record2;
    record2.push_back(std::make_unique<Field<int>>("id", 2));
    record2.push_back(std::make_unique<Field<std::string>>("name", "Bob"));
    record2.push_back(std::make_unique<Field<std::string>>("description", "L"));
    records.push_back(std::move(record2));

    // Add data to the table
    EXPECT_NO_THROW(db_client->insert(test_table, records));

    // Retrieve data and verify
    const auto results = db_client->select(test_table);

    EXPECT_EQ(results.size(), 2);

    // Check content
    for (const auto& rec : results)
    {
        const auto id = rec[0]->as<int32_t>();
        const auto name = rec[1]->as<std::string>();

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

TEST_F(PqxxClientTest, InsertSpeedTest)
{
    // Create sample data
    std::vector<Record> records;
    constexpr uint32_t flush = 1 << 14;
    constexpr uint32_t limit_ = flush * 8;
    records.reserve(flush);
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start();
    stopwatch.set_countdown_from_prev(true);
    for (uint32_t i = 1; i <= limit_; i++)
    {
        Record record1;
        record1.push_back(std::make_unique<Field<int32_t>>("id", i));
        record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
        record1.push_back(std::make_unique<Field<std::string>>("description", "P"));
        records.push_back(std::move(record1));
        if (records.size() >= flush)
        {
            EXPECT_NO_THROW(db_client->insert(test_table, std::move(records)));
            ++stopwatch;
            records.clear();
        }
    }
    stopwatch.finish();
    const auto results = db_client->select(test_table);

    EXPECT_EQ(results.size(), limit_);
}

TEST_F(PqxxClientTest, SelectSpeedTest)
{
    // Create sample data
    std::vector<Record> records;
    constexpr uint32_t flush = 1 << 14;
    constexpr uint32_t limit_ = flush * 8;
    records.reserve(flush);

    for (uint32_t i = 1; i <= limit_; i++)
    {
        Record record1;
        record1.push_back(std::make_unique<Field<int32_t>>("id", i));
        record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
        record1.push_back(std::make_unique<Field<std::string>>("description", "Pers" + std::to_string(i)));
        records.push_back(std::move(record1));
        if (records.size() >= flush)
        {
            EXPECT_NO_THROW(db_client->insert(test_table, std::move(records)));
            records.clear();
        }
    }
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start();
    stopwatch.set_countdown_from_prev(true);

    auto results = db_client->select(test_table);
    EXPECT_EQ(results.size(), limit_);
    stopwatch.flag("Select all: " + std::to_string(limit_));
    FieldConditions conditions;
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int>>("id", 0),
        ">",
        std::make_unique<Field<int>>("", 1)
    ));
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int>>("id", 0),
        "<",
        std::make_unique<Field<int>>("", 30)
    ));

    results = db_client->select(test_table, conditions);
    stopwatch.flag("Select 28");
    EXPECT_EQ(results.size(), 28);
}

TEST_F(PqxxClientTest, FtsSpeedTest)
{
    // Create sample data
    std::vector<Record> records;
    constexpr uint32_t flush = 1 << 14;
    constexpr uint32_t limit_ = flush * 8;
    records.reserve(flush);

    for (uint32_t i = 1; i <= limit_; i++)
    {
        Record record1;
        record1.push_back(std::make_unique<Field<int32_t>>("id", i));
        record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
        record1.push_back(std::make_unique<Field<std::string>>("description", "Pers" + std::to_string(i % 3)));
        records.push_back(std::move(record1));
        if (records.size() >= flush)
        {
            EXPECT_NO_THROW(db_client->insert(test_table, std::move(records)));
            records.clear();
        }
    }
    drug_lib::common::Stopwatch<> stopwatch;
    stopwatch.start();
    stopwatch.set_countdown_from_prev(true);
    const std::string search_query = "Pers2";
    const auto results = db_client->get_data_fts(test_table, search_query);
    stopwatch.finish();
    EXPECT_EQ(results.size(), 43691);
}

TEST_F(PqxxClientTest, UpsertTestFullRecord)
{
    // Initial data
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int>>("id", 1));
    record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record1.push_back(std::make_unique<Field<std::string>>("description", ""));
    records.push_back(std::move(record1));

    // Add initial data
    EXPECT_NO_THROW(db_client->insert(test_table, records));

    // Upsert data
    std::vector<Record> upsert_records;

    Record record_upsert;
    record_upsert.push_back(std::make_unique<Field<int>>("id", 1));
    record_upsert.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record_upsert.push_back(std::make_unique<Field<std::string>>("description", "Alice Updated"));
    upsert_records.push_back(std::move(record_upsert));

    // Conflict and replace fields

    EXPECT_NO_THROW(
        db_client->upsert(test_table, upsert_records, {std::make_unique<Field<std::string>>("description", "")}));

    // Retrieve data and verify
    const auto results = db_client->select(test_table);

    EXPECT_EQ(results.size(), 1);

    auto& rec = results.front();
    const auto id = rec[0]->as<int32_t>();
    const auto name = rec[1]->as<std::string>();
    const auto description = rec[2]->as<std::string>();
    EXPECT_EQ(id, 1);
    EXPECT_EQ(name, "Alice");
    EXPECT_EQ(description, "Alice Updated");
}

TEST_F(PqxxClientTest, UpsertTestPartial)
{
    // Initial data
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int>>("id", 1));
    record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record1.push_back(std::make_unique<Field<std::string>>("description", ""));
    records.push_back(std::move(record1));

    // Add initial data
    EXPECT_NO_THROW(db_client->insert(test_table, records));

    // Upsert data
    std::vector<Record> upsert_records;

    Record record_upsert;
    record_upsert.push_back(std::make_unique<Field<int>>("id", 1));
    // CHANGES HERE //
    record_upsert.push_back(std::make_unique<Field<std::string>>("description", "Alice Updated"));
    upsert_records.push_back(std::move(record_upsert));

    // Conflict and replace fields

    EXPECT_NO_THROW(
        db_client->upsert(test_table, upsert_records, {std::make_unique<Field<std::string>>("description", "")}));

    const auto results = db_client->select(test_table);

    EXPECT_EQ(results.size(), 1);

    auto& rec = results.front();
    const auto id = rec[0]->as<int32_t>();
    const auto name = rec[1]->as<std::string>();
    const auto description = rec[2]->as<std::string>();
    EXPECT_EQ(id, 1);
    EXPECT_EQ(name, "Alice");
    EXPECT_EQ(description, "Alice Updated");
}

TEST_F(PqxxClientTest, RemoveTest)
{
    // Add data
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int>>("id", 1));
    record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record1.push_back(std::make_unique<Field<std::string>>("description", ""));
    records.push_back(std::move(record1));

    EXPECT_NO_THROW(db_client->insert(test_table, records));

    // Remove data
    FieldConditions conditions;
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        "=",
        std::make_unique<Field<int32_t>>("", 1)
    ));

    EXPECT_NO_THROW(db_client->remove(test_table, conditions));

    // Verify removal
    const auto results = db_client->select(test_table);
    EXPECT_TRUE(results.empty());
}

TEST_F(PqxxClientTest, CountTest)
{
    // Add data
    std::vector<Record> records;

    for (int i = 1; i <= 5; ++i)
    {
        Record record;
        record.push_back(std::make_unique<Field<int>>("id", i));
        record.push_back(std::make_unique<Field<std::string>>("name", "User" + std::to_string(i)));
        record.push_back(std::make_unique<Field<std::string>>("description", ""));
        records.push_back(std::move(record));
    }

    EXPECT_NO_THROW(db_client->insert(test_table, records));

    FieldConditions conditions;
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int>>("id", 0),
        "=",
        std::make_unique<Field<int>>("", 1)
    ));
    const uint32_t count = db_client->count(test_table, conditions);
    EXPECT_EQ(count, 1);
}

TEST_F(PqxxClientTest, CountAllTest)
{
    // Add data
    std::vector<Record> records;

    for (int i = 1; i <= 5; ++i)
    {
        Record record;
        record.push_back(std::make_unique<Field<int>>("id", i));
        record.push_back(std::make_unique<Field<std::string>>("name", "User" + std::to_string(i)));
        record.push_back(std::make_unique<Field<std::string>>("description", ""));
        records.push_back(std::move(record));
    }

    EXPECT_NO_THROW(db_client->insert(test_table, records));
    const uint32_t count_all = db_client->count(test_table);
    EXPECT_EQ(count_all, 5);
}

TEST_F(PqxxClientTest, FullTextSearchTest)
{
    // Add data to the table
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int>>("id", 1));
    record1.push_back(std::make_unique<Field<std::string>>("name", "Apple"));
    record1.push_back(std::make_unique<Field<std::string>>("description", "A sweet red fruit"));
    records.push_back(std::move(record1));

    Record record2;
    record2.push_back(std::make_unique<Field<int>>("id", 2));
    record2.push_back(std::make_unique<Field<std::string>>("name", "Banana"));
    record2.push_back(std::make_unique<Field<std::string>>("description", "A long yellow fruit"));
    records.push_back(std::move(record2));

    Record record3;
    record3.push_back(std::make_unique<Field<int>>("id", 3));
    record3.push_back(std::make_unique<Field<std::string>>("name", "Carrot"));
    record3.push_back(std::make_unique<Field<std::string>>("description", "An orange root vegetable"));
    records.push_back(std::move(record3));

    // Insert records into the database
    EXPECT_NO_THROW(db_client->insert(test_table, records));

    std::string search_query = "fruit";
    auto results = db_client->get_data_fts(test_table, search_query);

    // Verify the results
    EXPECT_EQ(results.size(), 2);

    std::set<int> expected_ids = {1, 2};
    std::set<int> result_ids;
    for (const auto& rec : results)
    {
        int id = rec[0]->as<int32_t>();
        result_ids.insert(id);
    }

    EXPECT_EQ(result_ids, expected_ids);

    // Search for 'yellow'
    search_query = "yellow";
    results = db_client->get_data_fts(test_table, search_query);

    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.front()[1]->as<std::string>(), "Banana");

    // Search for 'vegetable'
    search_query = "vegetable";
    results = db_client->get_data_fts(test_table, search_query);

    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.front()[1]->as<std::string>(), "Carrot");

    // Search for a term not present
    search_query = "berry";
    results = db_client->get_data_fts(test_table, search_query);

    EXPECT_TRUE(results.empty());
}

TEST_F(PqxxClientTest, TruncateTableTest)
{
    // Add data
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int>>("id", 1));
    record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record1.push_back(std::make_unique<Field<std::string>>("description", ""));
    records.push_back(std::move(record1));
    Record record2;
    record2.push_back(std::make_unique<Field<int>>("id", 2));
    record2.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
    record2.push_back(std::make_unique<Field<std::string>>("description", ""));
    records.push_back(std::move(record2));
    EXPECT_NO_THROW(db_client->insert(test_table, records));

    // Remove data
    FieldConditions conditions;
    conditions.add_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        "=",
        std::make_unique<Field<int32_t>>("", 1)
    ));

    EXPECT_NO_THROW(db_client->truncate(test_table));

    // Verify removal
    const auto results = db_client->select(test_table);
    EXPECT_TRUE(results.empty());
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
