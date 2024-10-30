# PARENT_PROJECT_NAME = DrugLib_Common_Database
set(MockDbClient ${PROJECT_NAME}_Common_Database_MockClient)

##############################################################################
# Mock Database client implementation used for testing purposes
##############################################################################
add_library(${MockDbClient}
        source/mock_db_client.cpp
        include/mock_db_client.hpp
)

target_link_libraries(${MockDbClient}
        PUBLIC
        JsonCpp::JsonCpp
        ${PROJECT_NAME}_Common_Database_Interface
        ${PROJECT_NAME}_Common_Database_Exceptions
        ${PROJECT_NAME}_Common_StopWatch
)

target_include_directories(${MockDbClient}
        PUBLIC
        include/
)
##############################################################################
