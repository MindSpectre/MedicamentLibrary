
##############################################################################
# Database: Behavioral: Operation Strategies for DB
##############################################################################
add_library(${PROJECT_NAME}_Common_Database_Behavioral_OperationsStrategies
        operations_strategies/controller/source/strategy_control.cpp
        operations_strategies/controller/include/strategy_control.hpp
)
target_include_directories(${PROJECT_NAME}_Common_Database_Behavioral_OperationsStrategies
        PUBLIC
        operations_strategies/controller/include
        operations_strategies/controller/source
        operations_strategies/interface
        operations_strategies/patterns
)
target_link_libraries(${PROJECT_NAME}_Common_Database_Behavioral_OperationsStrategies
        PUBLIC
        ${PROJECT_NAME}_Common_Database_Interface
)
##############################################################################


##############################################################################
# Database: Behavioral: Operation Strategies for DB
##############################################################################
add_library(${PROJECT_NAME}_Common_Database_Behavioral_TransactionManager
        transaction_manager/include/transaction_manager.hpp
        transaction_manager/source/transaction_manager.cpp
)
target_include_directories(${PROJECT_NAME}_Common_Database_Behavioral_TransactionManager
        PUBLIC
        transaction_manager/include
        transaction_manager/source
)
target_link_libraries(${PROJECT_NAME}_Common_Database_Behavioral_TransactionManager
        PUBLIC
        ${PROJECT_NAME}_Common_Database_Interface
)
##############################################################################