#include <gtest/gtest.h>
#include <thread>
#include <memory>
#include <boost/function/function_template.hpp>

#include "db_interface_pool.hpp"
#include "db_interface.hpp"
#include "mock_db_client.hpp"

using namespace drug_lib::common::database;

class DbInterfacePoolTest : public testing::Test
{
protected:
    creational::DbInterfacePool pool;

    // Custom factory function for testing
    static std::unique_ptr<interfaces::DbInterface> create_mock_database()
    {
        return std::make_unique<drug_lib::common::database::MockDbClient>();
    }

    void SetUp() override
    {
        // Fill the pool with mock instances for testing
        pool.fill(5, create_mock_database);
    }

    void TearDown() override
    {
        pool.clear();
    }
};

// Test acquiring and releasing an object
TEST_F(DbInterfacePoolTest, TestAcquireAndRelease)
{
    auto db_interface = pool.acquire_db_interface();
    EXPECT_NE(db_interface, nullptr); // Ensure an object was acquired

    // Release the object back to the pool
    pool.release_db_interface(std::move(db_interface));
    EXPECT_NO_THROW(pool.acquire_db_interface()); // Ensure it can be re-acquired
}

// Test pool exhaustion
TEST_F(DbInterfacePoolTest, TestPoolExhaustion)
{
    std::vector<std::unique_ptr<interfaces::DbInterface>> interfaces;

    // Acquire all available instances
    interfaces.reserve(5);
    for (int i = 0; i < 5; ++i)
    {
        interfaces.push_back(pool.acquire_db_interface());
    }

    // The pool should now be empty
    EXPECT_THROW(pool.acquire_db_interface(), std::runtime_error);
}

// Test filling the pool with a custom size
TEST_F(DbInterfacePoolTest, TestCustomFillSize)
{
    creational::DbInterfacePool customPool;
    customPool.fill(3, create_mock_database);

    // Test that we can acquire 3 instances without exhaustion
    for (int i = 0; i < 3; ++i)
    {
        EXPECT_NO_THROW(customPool.acquire_db_interface());
    }

    // Expect exhaustion after acquiring all 3
    EXPECT_THROW(customPool.acquire_db_interface(), std::runtime_error);
}

// Test that the factory function fails gracefully
TEST_F(DbInterfacePoolTest, TestFactoryFunctionFailure)
{
    creational::DbInterfacePool failingPool;

    // Custom factory function that returns nullptr to simulate failure
    auto failingFactory = []() -> std::unique_ptr<interfaces::DbInterface>
    {
        return nullptr;
    };

    // Expect the fill method to throw when the factory fails
    EXPECT_THROW(failingPool.fill(5, failingFactory), std::runtime_error);
}

// Test multithreaded acquire and release operations
TEST_F(DbInterfacePoolTest, TestMultiThreadedAcquireRelease)
{
    std::vector<std::thread> threads;

    // Launch multiple threads to acquire and release objects
    threads.reserve(10);
    std::mutex stream_mutex;
    for (int i = 0; i < 10; ++i)
    {
        threads.emplace_back([&]()
        {
            try
            {
                auto db_interface = pool.acquire_db_interface();
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate work
                pool.release_db_interface(std::move(db_interface));
            }
            catch (const std::runtime_error& e)
            {
                std::lock_guard lock(stream_mutex);
                std::cerr << e.what() << "Caught in thread: " << std::this_thread::get_id() << std::endl;
            }
        });
    }

    // Join all threads
    for (auto& thread : threads)
    {
        thread.join();
    }

    EXPECT_NO_THROW(pool.acquire_db_interface()); // Ensure pool is usable after multithreaded access
}

// Test that releasing null pointers does not affect the pool
TEST_F(DbInterfacePoolTest, TestReleaseNull)
{
    std::unique_ptr<interfaces::DbInterface> nullInterface;
    EXPECT_NO_THROW(pool.release_db_interface(std::move(nullInterface))); // Should not throw or affect the pool
}
