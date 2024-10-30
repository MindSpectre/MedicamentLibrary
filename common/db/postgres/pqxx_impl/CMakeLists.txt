# PARENT_PROJECT_NAME = DrugLib_Common_Database
set(PqxxClient ${PROJECT_NAME}_Common_Database_PqxxClient)

##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(${PqxxClient}
        source/pqxx_client.cpp
        include/pqxx_utilities.hpp
        include/pqxx_connect_params.hpp
        include/pqxx_view_record.hpp
        include/pqxx_controller.hpp
)

target_link_libraries(${PqxxClient}
        PUBLIC
        JsonCpp::JsonCpp
        ${PROJECT_NAME}_Common_Database_Interface
        ${PROJECT_NAME}_Common_Database_Exceptions
        ${PROJECT_NAME}_Common_Database_Behavioral_OperationsStrategies
        ${PROJECT_NAME}_Common_StopWatch
        libpqxx::pqxx
        Boost::container
)

target_include_directories(${PqxxClient}
        PUBLIC
        include/
)
##############################################################################
