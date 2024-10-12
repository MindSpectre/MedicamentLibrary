
##############################################################################
# Database: Behavioral: Operation Strategies for DB
##############################################################################
add_library(${DATABASE_LIBS}_Behavioral_OperationsStrategies
        operations_strategies/controller/source/strategy_control.cpp
        operations_strategies/controller/include/strategy_control.hpp
)
target_include_directories(${DATABASE_LIBS}_Behavioral_OperationsStrategies
        PUBLIC
        operations_strategies/controller/include
        operations_strategies/controller/source
        operations_strategies/interface
        operations_strategies/patterns
)
target_link_libraries(${DATABASE_LIBS}_Behavioral_OperationsStrategies
        PUBLIC
        ${DATABASE_LIBS}_Interface
)
##############################################################################