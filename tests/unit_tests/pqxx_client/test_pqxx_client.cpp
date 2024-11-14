// pqxx_client_test.cpp

#include <barrier>
#include <chrono>
#include <db_interface_factory.hpp>
#include <functional>
#include <gtest/gtest.h>

#include "db_conditions.hpp"
#include "db_field.hpp"
#include "db_record.hpp"
#include "pqxx_client.hpp"
#include "pqxx_utilities.hpp"
#include "stopwatch.hpp"
using namespace drug_lib::common::database;

class PqxxClientTest : public testing::Test
{
protected:
    // Database connection parameters
    //
    uint32_t port = 5432;
    const std::string_view host = "localhost";
    const std::string_view db_name = "test_db";
    const std::string_view username = "postgres";
    const std::string_view password = "postgres"; // Replace with your actual password

    // Pointer to the PqxxClient
    std::shared_ptr<interfaces::DbInterface> db_client;

    // Test table name
    std::string test_table = "test_table";

    // SetUp and TearDown methods
    void SetUp() override
    {
        // Initialize the database client
        db_client = creational::DbInterfaceFactory::create_pqxx_client({host, port, db_name, username, password});

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


        db_client->setup_fts_index(test_table, std::move(fts_fields));
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

// ------------------------------ BASIC TESTS ------------------------------//
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


TEST_F(PqxxClientTest, UpsertTestFullRecord)
{
    // Initial data
    std::vector<Record> records;

    Record record1;
    record1.push_back(std::make_unique<Field<int32_t>>("id", 1));
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
    Conditions conditions;
    conditions.add_field_condition(FieldCondition(
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

    Conditions conditions;
    conditions.add_field_condition(FieldCondition(
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
    Conditions conditions;
    conditions.add_pattern_condition(PatternCondition(search_query));
    auto results = db_client->select(test_table, conditions);

    // Verify the results
    EXPECT_EQ(results.size(), 2);

    std::set expected_ids = {1, 2};
    std::set<int> result_ids;
    for (const auto& rec : results)
    {
        int id = rec[0]->as<int32_t>();
        result_ids.insert(id);
    }

    EXPECT_EQ(result_ids, expected_ids);
    conditions.pop_pattern_condition();
    // Search for 'yellow'
    search_query = "yellow";
    conditions.add_pattern_condition(PatternCondition(search_query));
    results = db_client->select(test_table, conditions);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.front()[1]->as<std::string>(), "Banana");
    conditions.pop_pattern_condition();
    // Search for 'vegetable'
    search_query = "vegetable";
    conditions.add_pattern_condition(PatternCondition(search_query));
    results = db_client->select(test_table, conditions);
    EXPECT_EQ(results.size(), 1);
    EXPECT_EQ(results.front()[1]->as<std::string>(), "Carrot");
    conditions.pop_pattern_condition();
    // Search for a term not present
    search_query = "berry";
    conditions.add_pattern_condition(PatternCondition(search_query));
    results = db_client->select(test_table, conditions);

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
    Conditions conditions;
    conditions.add_field_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        "=",
        std::make_unique<Field<int32_t>>("", 1)
    ));

    EXPECT_NO_THROW(db_client->truncate_table(test_table));

    // Verify removal
    const auto results = db_client->select(test_table);
    EXPECT_TRUE(results.empty());
}

// ------------------------------ TRANSACTION TESTS ------------------------------//

TEST_F(PqxxClientTest, TransactionSimpleTest)
{
    EXPECT_NO_THROW(db_client->start_transaction());
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
    EXPECT_NO_THROW(db_client->commit_transaction());
    auto results = db_client->select(test_table);
    EXPECT_EQ(results.size(), 2);

    EXPECT_NO_THROW(db_client->truncate_table(test_table));

    EXPECT_EQ(db_client->select(test_table).size(), 0);
    // Test rollback
    EXPECT_NO_THROW(db_client->start_transaction());

    // Add data to the table
    EXPECT_NO_THROW(db_client->insert(test_table, records));

    EXPECT_NO_THROW(db_client->rollback_transaction());
    EXPECT_EQ(db_client->select(test_table).size(), 0);
}

TEST_F(PqxxClientTest, TransactionConquerenteTest)
{
    EXPECT_NO_THROW(db_client->start_transaction());
    EXPECT_THROW(db_client->start_transaction(), drug_lib::common::database::exceptions::TransactionException);
    EXPECT_NO_THROW(db_client->commit_transaction());
}

TEST_F(PqxxClientTest, TransactionMultithreadTest)
{
    std::mutex mtx;
    std::atomic inserting1(true), inserting2(true); // Tracks if the poster thread is still inserting data
    constexpr std::size_t expected_record_count = 1e5; // Expected number of rows to insert
    std::condition_variable cv;
    std::barrier barrier(2), syncer(2);
    std::atomic transaction_committed{false};
    std::atomic<bool> ready_to_listen = false;
    // First thread - poster: inserts records within a transaction
    auto poster_worker = [&]
    {
        EXPECT_NO_THROW(db_client->start_transaction());
        {
            std::unique_lock lock(mtx);
            ready_to_listen = true;
        }
        cv.notify_all();
        for (std::size_t i = 1; i <= expected_record_count; ++i)
        {
            std::vector<Record> records(1);
            records[0].push_back(std::make_unique<Field<int32_t>>("id", i));
            records[0].push_back(std::make_unique<Field<std::string>>("name", "Alice"));
            records[0].push_back(std::make_unique<Field<std::string>>("description", "Here will be something"));

            EXPECT_NO_THROW(db_client->insert(test_table, std::move(records)));
        }


        // Signal that inserting is done
        inserting1.store(false);
        inserting2.store(false);
        barrier.arrive_and_wait();
        EXPECT_NO_THROW(db_client->commit_transaction());
        transaction_committed.store(true); // Set the flag to indicate commit is done
    };

    // Second thread - listener 1: retrieves data without a transaction
    auto non_transaction_worker = [&]
    {
        {
            std::unique_lock lock(mtx);
            cv.wait(lock, [&] { return ready_to_listen.load(); });
        }
        while (inserting1.load())
        {
            bool emp = true;
            // This thread should always successfully retrieve data, even if incomplete
            EXPECT_NO_THROW({
                emp = db_client->view(test_table).empty(); // Replace with actual retrieval logic

                // Non-transactional fetch should not throw an error
                // Optionally, you can log or validate the number of records fetched
                });
            EXPECT_FALSE(emp);
        }
    };

    // Third thread - listener 2: tries to retrieve data inside a transaction
    auto transactional_listener_worker = [&]
    {
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [&] { return ready_to_listen.load(); });
        }
        while (inserting2.load())
        {
            EXPECT_THROW(db_client->start_transaction(),
                         drug_lib::common::database::exceptions::TransactionException);
        }
        barrier.arrive_and_wait(); // Wait for all threads to reach the barrier

        // Ensure that the transaction is committed by Thread 1 before proceeding
        while (!transaction_committed.load())
        {
            std::this_thread::yield(); // Yield to avoid busy waiting
        }

        // Now proceed with the transaction check
        EXPECT_NO_THROW(db_client->start_transaction());

        // Fetch all rows inserted by the poster thread
        auto records = db_client->view(test_table);
        EXPECT_EQ(records.size(), expected_record_count);

        EXPECT_NO_THROW(db_client->commit_transaction());
    };

    // Create and launch the threads
    std::thread poster_thread(poster_worker);
    std::thread non_transaction_thread(non_transaction_worker);
    std::thread transactional_listener_thread(transactional_listener_worker);

    // Join threads
    poster_thread.join();
    non_transaction_thread.join();
    transactional_listener_thread.join();
}

TEST_F(PqxxClientTest, OrderByTest)
{
    // Add data
    std::vector<Record> records;

    for (int i = 1; i <= 5; ++i)
    {
        Record record;
        record.push_back(std::make_unique<Field<int>>("id", i * (i + i % 2)));
        record.push_back(std::make_unique<Field<std::string>>("name", "User" + std::to_string(i)));
        record.push_back(std::make_unique<Field<std::string>>("description", ""));
        records.push_back(std::move(record));
    }

    EXPECT_NO_THROW(db_client->insert(test_table, records));

    Conditions conditions;
    conditions.add_order_by_condition(
        OrderCondition("id", order_type::ascending));
    const auto res = db_client->select(test_table, conditions);
    int32_t prev = INT_MIN;
    for (const auto& record : res)
    {
        EXPECT_TRUE(prev <= record[0]->as<int32_t>());
        prev = record[0]->as<int32_t>();
    }
    EXPECT_EQ(res.size(), 5);
}

TEST_F(PqxxClientTest, PagingTest)
{
    // Add data
    std::vector<Record> records;

    for (int i = 1; i <= 500; ++i)
    {
        Record record;
        record.push_back(std::make_unique<Field<int>>("id", i));
        record.push_back(std::make_unique<Field<std::string>>("name", "User" + std::to_string(i)));
        record.push_back(std::make_unique<Field<std::string>>("description", ""));
        records.push_back(std::move(record));
    }

    EXPECT_NO_THROW(db_client->insert(test_table, records));

    Conditions conditions;
    PageCondition page_condition(150);

    conditions.set_page_condition(page_condition);
    auto res = db_client->view(test_table, conditions);
    EXPECT_EQ(res.size(), 150);
    page_condition.set_page_number(500 / 150 + 1);
    conditions.set_page_condition(page_condition);
    res = db_client->view(test_table, conditions);
    EXPECT_EQ(res.size(), 50);
}

// ------------------------------ SPEED TESTS ------------------------------//

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

TEST_F(PqxxClientTest, InsertMultithreadSpeedTest)
{
    drug_lib::common::Stopwatch<> stopwatch;
    constexpr uint32_t flush = 1 << 14; // Number of records per flush
    constexpr uint32_t limit_ = flush * 2; // Total number of records each thread should handle
    constexpr uint32_t thread_count = 4; // Number of threads to use

    // Worker function to insert records in parallel
    auto poster_worker = [&](const int s)
    {
        std::vector<Record> records;
        records.reserve(flush); // Reserve space for records to avoid reallocations

        // stopwatch.flag("Thread " + std::to_string(s) + " started");

        // Each thread inserts records with step `thread_count` (to avoid overlapping IDs)
        for (uint32_t i = s; i <= limit_ * thread_count; i += thread_count)
        {
            Record record1;
            record1.push_back(std::make_unique<Field<int32_t>>("id", i));
            record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
            record1.push_back(std::make_unique<Field<std::string>>("description", "P"));
            records.push_back(std::move(record1));

            // Insert the records into the database in batches when flush limit is reached
            if (records.size() >= flush)
            {
                EXPECT_NO_THROW(db_client->insert(test_table, std::move(records)));
                records.clear();
            }
        }

        // stopwatch.flag("Thread " + std::to_string(s) + " finished");
    };

    // Launch threads
    stopwatch.start("Threading launch: 4 threads");
    std::vector<std::thread> threads;
    threads.reserve(thread_count);
    for (int t = 0; t < thread_count; ++t)
    {
        threads.emplace_back(poster_worker, t); // Each thread handles a different starting point
    }

    // Join threads
    for (auto& thread : threads)
    {
        thread.join();
    }
    stopwatch.flag("Threading finished: 4 threads");
    // Fetch the records from the database
    const auto results = db_client->view(test_table);
    stopwatch.finish();

    // Check that the correct number of records were inserted
    EXPECT_EQ(results.size(), limit_ * thread_count);
}

TEST_F(PqxxClientTest, InsertMultithreadSpeedTestWithDropppingFts)
{
    std::vector<std::shared_ptr<FieldBase>> fts_fields;
    fts_fields.emplace_back(std::make_shared<Field<std::string>>("name", ""));
    fts_fields.emplace_back(std::make_shared<Field<std::string>>("description", ""));


    drug_lib::common::Stopwatch<> stopwatch;
    constexpr uint32_t flush = 1 << 14; // Number of records per flush
    constexpr uint32_t limit_ = flush * 8; // Total number of records each thread should handle
    db_client->drop_fts_index(test_table);
    std::vector<Record> records;
    records.reserve(limit_); // Reserve space for records to avoid reallocations
    for (uint32_t i = 0; i < limit_; i++)
    {
        Record record1;
        record1.push_back(std::make_unique<Field<int32_t>>("id", i));
        record1.push_back(std::make_unique<Field<std::string>>("name", "Alice"));
        record1.push_back(std::make_unique<Field<std::string>>("description", "Pers" + std::to_string(i % 3)));
        records.push_back(std::move(record1));
    }
    stopwatch.start("Multithreading insert with dropping fts");
    stopwatch.flag("Threading launch: 6 threads");
    // stopwatch.flag("Thread " + std::to_string(s) + " finished");
    const std::shared_ptr<PqxxClient> db_client_n = std::dynamic_pointer_cast<PqxxClient>(db_client);
    utilities::bulk_insertion(db_client_n, test_table, std::move(records), flush, 6);
    stopwatch.flag("Threading finished: 6 threads");
    // Fetch the records from the database
    const auto results = db_client->view(test_table);
    stopwatch.flag("Viewed");
    // Check that the correct number of records were inserted
    EXPECT_EQ(results.size(), limit_);
    Conditions conditions;
    conditions.add_pattern_condition(PatternCondition("Pers2"));
    const auto fts_res = db_client->select(test_table, conditions);
    stopwatch.finish();
    EXPECT_EQ(fts_res.size(), limit_/3);
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
    stopwatch.start("Selecting with ready fields");
    stopwatch.set_countdown_from_prev(true);

    auto results = db_client->select(test_table);
    EXPECT_EQ(results.size(), limit_);
    stopwatch.flag("Select all: " + std::to_string(limit_));
    Conditions conditions;
    constexpr int32_t a = 1, b = 30;
    conditions.add_field_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        ">=",
        std::make_unique<Field<int32_t>>("", a)
    ));
    conditions.add_field_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        "<",
        std::make_unique<Field<int32_t>>("", b)
    ));

    results = db_client->select(test_table, conditions);
    stopwatch.flag("Select with conditions: " + std::to_string(b - a));
    EXPECT_EQ(results.size(), b-a);

    Conditions conditions_page;
    constexpr uint32_t page_sz = 1000;
    conditions_page.set_page_condition(PageCondition(page_sz));
    auto page_res = db_client->select(test_table, conditions_page);
    stopwatch.flag("View with paging");
    EXPECT_EQ(page_res.size(), page_sz);
}


TEST_F(PqxxClientTest, ViewSpeedTest)
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
    stopwatch.start("View speed with transforming");

    const auto results = db_client->view(test_table);
    stopwatch.flag("View all: " + std::to_string(limit_));
    Record x;
    x.reserve(limit_);
    for (const auto& record : results)
    {
        auto f = record->extract(0);
        auto f1 = record->extract(1);
        auto f2 = record->extract(2);
        x.push_back(std::make_unique<Field<std::string>>("id", f));
        x.push_back(std::make_unique<Field<std::string>>("name", f1));
        x.push_back(std::make_unique<Field<std::string>>("description", f2));
    }
    EXPECT_EQ(results.size(), limit_);
    stopwatch.flag("View all + transformed: " + std::to_string(limit_));
    Conditions conditions;
    constexpr int32_t a = 1, b = 30;
    conditions.add_field_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        ">=",
        std::make_unique<Field<int32_t>>("", a)
    ));
    conditions.add_field_condition(FieldCondition(
        std::make_unique<Field<int32_t>>("id", 0),
        "<",
        std::make_unique<Field<int32_t>>("", b)
    ));

    const auto res2 = db_client->view(test_table, conditions);

    stopwatch.flag("View with conditions: " + std::to_string(b - a));
    EXPECT_EQ(res2.size(), b-a);
    Record x2;
    x2.reserve(limit_);
    for (const auto& record : res2)
    {
        auto f = record->extract(0);
        auto f1 = record->extract(1);
        auto f2 = record->extract(2);
        x2.push_back(std::make_unique<Field<std::string>>("id", f));
        x2.push_back(std::make_unique<Field<std::string>>("name", f1));
        x2.push_back(std::make_unique<Field<std::string>>("description", f2));
    }
    stopwatch.flag("View with conditions + transformed: " + std::to_string(b - a));

    Conditions conditions_page;
    constexpr uint32_t page_sz = 1000;
    conditions_page.set_page_condition(PageCondition(page_sz));
    auto page_res = db_client->view(test_table, conditions_page);
    stopwatch.flag("View with paging");
    EXPECT_EQ(page_res.size(), page_sz);
    x2.clear();
    x2.reserve(limit_);
    for (const auto& record : page_res)
    {
        auto f = record->extract(0);
        auto f1 = record->extract(1);
        auto f2 = record->extract(2);
        x2.push_back(std::make_unique<Field<std::string>>("id", f));
        x2.push_back(std::make_unique<Field<std::string>>("name", f1));
        x2.push_back(std::make_unique<Field<std::string>>("description", f2));
    }
    stopwatch.flag("View with paging + transformed");
}

TEST_F(PqxxClientTest, FtsSpeedTest)
{
    // Create sample data
    std::vector<Record> records;
    constexpr uint32_t flush = 1 << 14;
    constexpr uint32_t limit_ = flush * 8;
    records.reserve(flush);

    for (uint32_t i = 0; i < limit_; i++)
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
    Conditions conditions;
    conditions.add_pattern_condition(PatternCondition(search_query));
    const auto results = db_client->view(test_table, conditions);
    stopwatch.finish();
    EXPECT_EQ(results.size(), limit_/3);
}


int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
