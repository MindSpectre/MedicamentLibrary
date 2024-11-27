##############################################################################
# Postgres Database client implementation used on PQXX lib ONLY FOR C++
##############################################################################
add_library(DrugLib_Common_Database_PqxxClient
        source/pqxx_client.cpp
        include/pqxx_utilities.hpp
        include/pqxx_connect_params.hpp
        include/pqxx_view_record.hpp
        include/pqxx_controller.hpp
)

target_link_libraries(DrugLib_Common_Database_PqxxClient
        PUBLIC
        JsonCpp::JsonCpp
        DrugLib_Common_Database_Interface
        DrugLib_Common_Database_Exceptions
        DrugLib_Common_Database_Behavioral_OperationsStrategies
        DrugLib_Common_StopWatch
        libpqxx::pqxx
        Boost::container
)

target_include_directories(DrugLib_Common_Database_PqxxClient
        PUBLIC
        include/
)
##############################################################################
