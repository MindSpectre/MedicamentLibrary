##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(DrugLib_Common_StopWatch
        source/stopwatch.cpp
        include/stopwatch.hpp
)
target_include_directories(DrugLib_Common_StopWatch
        PUBLIC
        include/
)
##############################################################################