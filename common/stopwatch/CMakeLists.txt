##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(${COMMON_LIBS}_StopWatch
        source/stopwatch.cpp
        include/stopwatch.hpp
)

target_link_libraries(${COMMON_LIBS}_StopWatch
        PUBLIC
)

target_include_directories(${COMMON_LIBS}_StopWatch
        PUBLIC
        source/
        include/
)
##############################################################################