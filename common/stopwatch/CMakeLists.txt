##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(${PROJECT_NAME}_Common_StopWatch
        source/stopwatch.cpp
        include/stopwatch.hpp
)

target_link_libraries(${PROJECT_NAME}_Common_StopWatch
        PUBLIC
)

target_include_directories(${PROJECT_NAME}_Common_StopWatch
        PUBLIC
        source/
        include/
)
##############################################################################