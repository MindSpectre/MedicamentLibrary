# PARENT_PROJECT_NAME = DrugLib_Common_Database
set(TARGET ${DATABASE_LIBS}_PqxxClient)

##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(${TARGET}
        source/pqxx_client.cpp
)

target_link_libraries(${TARGET}
        PUBLIC
        JsonCpp::JsonCpp
        ${DATABASE_LIBS}_Interface
        ${DATABASE_LIBS}_Exceptions
        ${DATABASE_LIBS}_Behavioral_OperationsStrategies
        libpqxx::pqxx
        Boost::container
)

target_include_directories(${TARGET}
        PUBLIC
        source/
        include/
)
##############################################################################
