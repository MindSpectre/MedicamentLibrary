##############################################################################
# Test PQxx Client DB
##############################################################################
add_executable(${INTEGRATION_TESTING_TARGET}_PqxxClient
        pqxx_client/test_pqxx_client.cpp
)
target_link_libraries(${INTEGRATION_TESTING_TARGET}_PqxxClient
        PRIVATE
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_Factory
        DrugLib_Common_StopWatch
        ${TEST_NECESSARY_LIBS}

)
add_test(UnitTest_PqxxClient ${INTEGRATION_TESTING_TARGET}_PqxxClient)
set_tests_properties(UnitTest_PqxxClient PROPERTIES LABELS "integration")
##############################################################################