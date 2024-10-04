##############################################################################
# Database interface Factory class
##############################################################################

add_library(${DATABASE_LIBS}_Factory
        STATIC
        factory/db_interface_factory.cpp

)
target_link_libraries(${DATABASE_LIBS}_Factory
        PUBLIC
        ${DATABASE_LIBS}_Interface
        ${DATABASE_LIBS}_PqxxClient
)
##############################################################################