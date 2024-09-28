# PARENT_PROJECT_NAME = DrugLib.Common.Database

set(TARGET_PREFIX ${DATABASE_LIBS}_Behavioral)

add_library(${TARGET_PREFIX}_OperationsStrategies
        operations_strategies/controller/source/strategy_control.cpp
        operations_strategies/controller/include/strategy_control.hpp
)
target_include_directories(${TARGET_PREFIX}_OperationsStrategies
        PUBLIC
        operations_strategies/controller/include
        operations_strategies/controller/source
        operations_strategies/interface
        operations_strategies/patterns
)
target_link_libraries(${TARGET_PREFIX}_OperationsStrategies
        PUBLIC
        ${DATABASE_LIBS}_Interface
)
