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

add_subdirectory(interfaces/exceptions)

add_subdirectory(interfaces/interface)

add_subdirectory(creational/factory)

#add_subdirectory(managers)

add_subdirectory(postgres/pqxx_impl)


