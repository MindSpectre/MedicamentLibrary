add_executable(InstallmentCheck
        basic/installment_check.cpp
)

target_link_libraries(InstallmentCheck
        PRIVATE
        DrugLib_Dao_CrudInterface
        #        DrugLib.Dao.Handbook.Medicaments
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_Behavioral_OperationsStrategies
)

SET(UNIT_TESTING_TARGET ${PROJECT_NAME}_Tests_Unit)

##############################################################################
# Test PQxx Client DB
##############################################################################
add_executable(${UNIT_TESTING_TARGET}_PqxxClient
        unit_tests/pqxx_client/test_pqxx_client.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}_PqxxClient
        PRIVATE
        DrugLib_Common_Database_PqxxClient
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
add_test(DbUnitTests ${UNIT_TESTING_TARGET}_PqxxClient)
##############################################################################
