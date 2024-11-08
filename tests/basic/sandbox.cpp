#include <iostream>

#include "db_field.hpp"
#include "stopwatch.hpp"
#include <chrono>

int main()
{
    drug_lib::common::Stopwatch<std::chrono::microseconds> timer;
    timer.start();
    timer.set_countdown_from_start(false);
    timer.set_countdown_from_prev(true);
    for (int i = 0; i < 1e7; i++)
    {
        if (i % 100000 == 0)
        {
            std::cout << i << std::endl;
            ++timer;
        }
    }
    timer.finish();
    return 0;
}
