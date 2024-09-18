set(PARENT_PROJECT_NAME ${PARENT_PROJECT_NAME}.Database)

add_subdirectory(interfaces/exceptions)

add_subdirectory(interfaces/transaction_manager)

add_subdirectory(postgres/pqxx_impl)

add_library(${PARENT_PROJECT_NAME}.Base INTERFACE)

target_include_directories(${PARENT_PROJECT_NAME}.Base
    INTERFACE
    base
)