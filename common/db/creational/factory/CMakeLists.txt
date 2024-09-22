# PARENT_PROJECT_NAME = DrugLib.Common.Database

set(UNITED_TARGET ${PARENT_PROJECT_NAME}.Factory)

##############################################################################
# Database interface Factory class
##############################################################################

add_library(UNITED_TARGET
        STATIC
        db_interface_factory.cpp

)
target_link_libraries(UNITED_TARGET
        PUBLIC
        DrugLib.Common.Database.DBI
        DrugLib.Common.Database.PqxxClient
)
##############################################################################