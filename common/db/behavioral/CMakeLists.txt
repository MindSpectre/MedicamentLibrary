# PARENT_PROJECT_NAME = DrugLib.Common.Database

set(TARGET_PREFIX ${PARENT_PROJECT_NAME}.Behavioral)

add_library(${TARGET_PREFIX}.OperationsStrategies
        operations_strategies/controller/source/strategy_control.cpp
        operations_strategies/controller/include/strategy_control.hpp
)
target_include_directories(${TARGET_PREFIX}.OperationsStrategies
        PUBLIC
        operations_strategies/controller/include
        operations_strategies/controller/source
        operations_strategies/interface
        operations_strategies/patterns
)
target_link_libraries(${TARGET_PREFIX}.OperationsStrategies
        PUBLIC
        DrugLib.Common.Database.Interface
)
