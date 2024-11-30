##############################################################################
# Test Stopwatch benchmark
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_StopWatch
        stopwatch/test_stopwatch.cpp
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
        transaction_manager/test_transaction_manager.cpp
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
        db_interface_pool/test_db_interface_pool.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_DbInterfacePool
        PRIVATE
        DrugLib_Common_Database_Pool
        DrugLib_Common_Database_Factory
        ${TEST_NECESSARY_LIBS}

)
add_test(UnitTest_DbInterfacePool ${UNIT_TESTING_TARGET}_DbInterfacePool)
##############################################################################

##############################################################################
# Objects and their properties
##############################################################################
add_subdirectory(objects)
##############################################################################

set_tests_properties(UnitTest_StopWatch UnitTest_TransactionManager UnitTest_DbInterfacePool PROPERTIES LABELS "unit")