##############################################################################
# Database interface Factory class
##############################################################################

add_library(${PROJECT_NAME}_Common_Database_Factory
        STATIC
        factory/db_interface_factory.cpp

)
target_link_libraries(${PROJECT_NAME}_Common_Database_Factory
        PUBLIC
        ${PROJECT_NAME}_Common_Database_Interface
        ${PROJECT_NAME}_Common_Database_PqxxClient
)
##############################################################################