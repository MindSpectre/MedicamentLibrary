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

SET(UNIT_TESTING_TARGET "${PROJECT_NAME}_Tests_Unit")
SET(TEST_NECESSARY_LIBS GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)


##############################################################################
# 1st Priority tests(Fast)
##############################################################################
add_subdirectory(unit_tests/objects)
##############################################################################

##############################################################################
# Test PQxx Client DB
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_PqxxClient
        unit_tests/pqxx_client/test_pqxx_client.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_PqxxClient
        PRIVATE
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_Factory
        DrugLib_Common_StopWatch
        ${TEST_NECESSARY_LIBS}

)
add_test(UnitTest_PqxxClient ${UNIT_TESTING_TARGET}_PqxxClient)
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
add_test(UnitTest_StopWatch ${UNIT_TESTING_TARGET}_StopWatch)
##############################################################################

##############################################################################
# Test Transaction Manager
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_TransactionManager
        unit_tests/transaction_manager/test_transaction_manager.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_TransactionManager
        PRIVATE
        DrugLib_Common_Database_Behavioral_TransactionManager
        DrugLib_Common_Database_Factory
        ${TEST_NECESSARY_LIBS}

)
add_test(UnitTest_TransactionManager ${UNIT_TESTING_TARGET}_TransactionManager)
##############################################################################

##############################################################################
# Test DbInterface Pool
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_DbInterfacePool
        unit_tests/db_interface_pool/test_db_interface_pool.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_DbInterfacePool
        PRIVATE
        DrugLib_Common_Database_Pool
        ${TEST_NECESSARY_LIBS}

)
add_test(UnitTest_DbInterfacePool ${UNIT_TESTING_TARGET}_DbInterfacePool)
##############################################################################