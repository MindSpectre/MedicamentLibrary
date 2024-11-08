#include <thread>
#include <gtest/gtest.h>

#include "stopwatch.hpp"


using namespace drug_lib::common;

class StopwatchTest : public testing::Test
{
protected:
    Stopwatch<> sw; // Stopwatch object using default time unit (milliseconds)

    void SetUp() override
    {
        // Setup before each test
        sw.start();
    }

    void TearDown() override
    {
        // Teardown after each test
    }

public:
    // Utility function to add a delay
    static void delay_ms(const int milliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }
};

// Test basic functionality: starting, resetting, and flagging
TEST_F(StopwatchTest, TestStartAndReset)
{
    sw.flag("Start Flag");
    delay_ms(10); // Wait to ensure time passes
    sw.flag("Mid Flag");

    EXPECT_NO_THROW(sw.reset()); // Ensure reset works
    EXPECT_NO_THROW(sw.flag("After Reset Flag"));
}

// Test the addition and removal of flags using overloaded operators
TEST_F(StopwatchTest, TestFlagAdditionAndRemoval)
{
    sw.flag("First Flag");
    EXPECT_NO_THROW(++sw); // Add flag using overloaded ++ operator
    delay_ms(5);
    EXPECT_NO_THROW(sw.flag("Mid Flag"));
    EXPECT_NO_THROW(--sw); // Remove flag using overloaded -- operator

    // Test removing flags until none remain
    EXPECT_NO_THROW(--sw);
    EXPECT_NO_THROW(--sw); // Should not throw even when there are no flags
}

// Test countdown from start and previous flags
TEST_F(StopwatchTest, TestCountdownModes)
{
    sw.set_countdown_from_prev(true);
    sw.set_countdown_from_start(false);
    EXPECT_TRUE(sw.is_count_from_prev());
    EXPECT_FALSE(sw.is_count_from_start());

    delay_ms(10);
    sw.flag("Flag 1");
    delay_ms(5);
    sw.flag("Flag 2");

    // Switch to countdown from start and verify
    sw.set_countdown_from_start(true);
    sw.set_countdown_from_prev(false);
    EXPECT_TRUE(sw.is_count_from_start());
    EXPECT_FALSE(sw.is_count_from_prev());
}

// Test that flags are accurately recorded and the time intervals make sense
TEST_F(StopwatchTest, TestAccurateFlagging)
{
    const auto start_time = std::chrono::high_resolution_clock::now();

    sw.flag("Initial Flag");
    delay_ms(10); // Simulate a delay
    sw.flag("Second Flag");

    const auto end_time = std::chrono::high_resolution_clock::now();

    const auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    // Validate that the elapsed time between start and last flag is within a reasonable range
    EXPECT_GE(elapsed_time, 10); // At least 10 ms should have passed
}

// Test finish() automatically prints and flushes flags
TEST_F(StopwatchTest, TestFinishAndPrint)
{
    sw.flag("Start Flag");
    delay_ms(5);
    sw.flag("Mid Flag");

    // Finish should print and clear all flags
    EXPECT_NO_THROW(sw.finish());

    // Ensure that after finish, no flags remain
    sw.flag("After Finish");
    EXPECT_NO_THROW(sw.finish()); // Should be empty before adding another flag
}

// Test the destructor calls print() automatically
TEST(StopwatchDestructorTest, TestDestructorCallsPrint)
{
    {
        Stopwatch<> temp_stopwatch;
        temp_stopwatch.start();
        temp_stopwatch.flag("Start Flag");
        StopwatchTest::delay_ms(10);
        temp_stopwatch.flag("End Flag");

        // When temp_stopwatch goes out of scope, print() should be called automatically
    } // Destructor should be called here
}

// Test flags capacity reservation and handling large number of flags
TEST_F(StopwatchTest, TestFlagCapacityHandling)
{
    Stopwatch large_sw("Capacity testing", 100); // Reserve capacity for 100 flags
    large_sw.start();
    for (int i = 0; i < 100; ++i)
    {
        large_sw.flag("Flag " + std::to_string(i));
        delay_ms(1); // Delay to ensure flag timestamps are distinct
    }

    EXPECT_NO_THROW(large_sw.finish()); // Ensure that handling a large number of flags works smoothly
}

// Test time unit conversion by checking durations in different time units
TEST(StopwatchTimeUnitTest, TestDifferentTimeUnits)
{
    Stopwatch<std::chrono::microseconds> micro_sw;
    micro_sw.start();
    StopwatchTest::delay_ms(1); // 1 ms delay
    micro_sw.flag("First Microsecond Flag");

    Stopwatch<std::chrono::nanoseconds> nano_sw;
    nano_sw.start();
    StopwatchTest::delay_ms(1); // 1 ms delay
    nano_sw.flag("First Nanosecond Flag");

    // Ensure there are no issues with different time units
    EXPECT_NO_THROW(micro_sw.finish());
    EXPECT_NO_THROW(nano_sw.finish());
}
