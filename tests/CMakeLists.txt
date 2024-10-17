add_executable(Sandbox
        basic/sandbox.cpp
)

target_link_libraries(Sandbox
        PRIVATE
        DrugLib_Dao_CrudInterface
        #        DrugLib.Dao.Handbook.Medicaments
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_Behavioral_OperationsStrategies
        DrugLib_Common_StopWatch
)

SET(UNIT_TESTING_TARGET ${PROJECT_NAME}_Tests_Unit)
SET(TEST_NECESSARY_LIBS GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
##############################################################################
# Test PQxx Client DB
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_PqxxClient
        unit_tests/pqxx_client/test_pqxx_client.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_PqxxClient
        PRIVATE
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_StopWatch
        ${TEST_NECESSARY_LIBS}

)
add_test(DbUnitTests ${UNIT_TESTING_TARGET}_PqxxClient)
##############################################################################

##############################################################################
# Test Stopwatch benchmark
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_StopWatch
        unit_tests/stopwatch/test_stopwatch.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_StopWatch
        PRIVATE
        DrugLib_Common_StopWatch
        ${TEST_NECESSARY_LIBS}

)
add_test(SupportClasses ${UNIT_TESTING_TARGET}_StopWatch)
##############################################################################