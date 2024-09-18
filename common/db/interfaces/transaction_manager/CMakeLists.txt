set(TARGET ${PARENT_PROJECT_NAME}.TransactionManager)

add_library(${TARGET}
    source/transaction_manager.cpp
)
target_include_directories(${TARGET}
    PUBLIC
    source/
    include/
    ${CMAKE_HOME_DIRECTORY}/common
)
target_link_libraries(${TARGET}
    JsonCpp::JsonCpp
    DrugLib.Common.Database.DBI
)
