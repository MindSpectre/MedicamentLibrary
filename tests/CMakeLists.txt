add_executable(Sandbox
        sandbox/sandbox.cpp
)

target_link_libraries(Sandbox
        PRIVATE
        DrugLib_Dao_Handbook_Base
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_Behavioral_OperationsStrategies
        DrugLib_Common_Stopwatch
        Drogon::Drogon
        JsonCpp::JsonCpp
)
add_executable(HTest_SearchService non_automate/search_service.cpp)

target_link_libraries(HTest_SearchService
        DrugLib_Services_Internal_Search
)


SET(UNIT_TESTING_TARGET ${PROJECT_NAME}_Tests_Unit)
SET(INTEGRATION_TESTING_TARGET ${PROJECT_NAME}_Tests_Integration)
SET(TEST_NECESSARY_LIBS GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main Drogon::Drogon)


add_subdirectory(unit_tests)
add_subdirectory(integration_tests)
