# PARENT_PROJECT_NAME = DrugLib.Common

set(PARENT_PROJECT_NAME ${PARENT_PROJECT_NAME}.Database)

##############################################################################
# Database main classes NECESSARY UTILITY
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Base INTERFACE)

target_include_directories(${PARENT_PROJECT_NAME}.Base
        INTERFACE
        base
)
##############################################################################

##############################################################################
# Database Client Interface
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Interface
        INTERFACE
        interface/db_interface.hpp
)

target_link_libraries(${PARENT_PROJECT_NAME}.Interface
        INTERFACE
        JsonCpp::JsonCpp
        DrugLib.Common.Database.Base
)
target_include_directories(${PARENT_PROJECT_NAME}.Interface
        INTERFACE
        interface
)
##############################################################################

##############################################################################
# Error handling
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Exceptions INTERFACE
        exceptions/exceptions.hpp
        exceptions/error_codes.hpp)
target_include_directories(${PARENT_PROJECT_NAME}.Exceptions INTERFACE
        exceptions
)
##############################################################################

##############################################################################


add_subdirectory(creational)

add_subdirectory(behavioral)

add_subdirectory(postgres/pqxx_impl)


