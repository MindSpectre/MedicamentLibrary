# PARENT_PROJECT_NAME = DrugLib_Common_Database
set(PqxxClient ${DATABASE_LIBS}_PqxxClient)

##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(${PqxxClient}
        source/pqxx_client.cpp
)

target_link_libraries(${PqxxClient}
        PUBLIC
        JsonCpp::JsonCpp
        ${DATABASE_LIBS}_Interface
        ${DATABASE_LIBS}_Exceptions
        ${DATABASE_LIBS}_Behavioral_OperationsStrategies
        ${COMMON_LIBS}_StopWatch
        libpqxx::pqxx
        Boost::container
)

target_include_directories(${PqxxClient}
        PUBLIC
        source/
        include/
)
##############################################################################
