# PARENT_PROJECT_NAME = DrugLib_Common_Database

set(UNITED_TARGET ${DATABASE_LIBS}_Factory)

##############################################################################
# Database interface Factory class
##############################################################################

add_library(${UNITED_TARGET}
        STATIC
        factory/db_interface_factory.cpp

)
target_link_libraries(${UNITED_TARGET}
        PUBLIC
        ${DATABASE_LIBS}_Interface
        ${DATABASE_LIBS}_PqxxClient
)
##############################################################################