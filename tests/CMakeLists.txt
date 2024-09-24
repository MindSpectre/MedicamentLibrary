add_executable(InstallmentCheck
        basic/installment_check.cpp
)

target_link_libraries(InstallmentCheck
        PRIVATE
        DrugLib.Dao.CrudInterface
        #        DrugLib.Dao.Handbook.Medicaments
        DrugLib.Common.Database.PqxxClient
        DrugLib.Common.Database.Behavioral.OperationsStrategies
)

SET(UNIT_TESTING_TARGET "DrugLib.Tests.Unit")
#enable_testing()

add_executable(${UNIT_TESTING_TARGET}.PqxxClient
        unit_tests/pqxx_client/test_pqxx_client.cpp
)
target_link_libraries(${UNIT_TESTING_TARGET}.PqxxClient
        PRIVATE
        DrugLib.Common.Database.PqxxClient
        GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)
add_test(DbUnitTests ${UNIT_TESTING_TARGET}.PqxxClient)