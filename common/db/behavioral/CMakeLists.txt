# PARENT_PROJECT_NAME = DrugLib.Common.Database

set(PARENT_PROJECT_NAME ${PARENT_PROJECT_NAME}.Managers)

##############################################################################
# Transaction Manager
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Transaction
        transactions/source/transaction_manager.cpp
)
target_include_directories(${PARENT_PROJECT_NAME}.Transaction
        PUBLIC
        transactions/source/
        transactions/include/
        ${CMAKE_HOME_DIRECTORY}/common
)
target_link_libraries(${PARENT_PROJECT_NAME}.Transaction
        JsonCpp::JsonCpp
        DrugLib.Common.Database.DBI
)
##############################################################################

##############################################################################

add_subdirectory(strategies)
