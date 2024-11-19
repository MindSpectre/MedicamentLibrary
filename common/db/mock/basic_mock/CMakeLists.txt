##############################################################################
# Mock Database client implementation used for testing purposes
##############################################################################
add_library(DrugLib_Common_Database_MockClient
        source/mock_db_client.cpp
        include/mock_db_client.hpp
)

target_link_libraries(DrugLib_Common_Database_MockClient
        PUBLIC
        JsonCpp::JsonCpp
        DrugLib_Common_Database_Interface
        DrugLib_Common_Database_Exceptions
        DrugLib_Common_StopWatch
)

target_include_directories(DrugLib_Common_Database_MockClient
        PUBLIC
        include/
)
##############################################################################
