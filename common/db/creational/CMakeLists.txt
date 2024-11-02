##############################################################################
# Database interface Factory class
##############################################################################

add_library(${PROJECT_NAME}_Common_Database_Factory
        STATIC
        factory/db_interface_factory.cpp
        factory/db_interface_factory.hpp
)
target_link_libraries(${PROJECT_NAME}_Common_Database_Factory
        PUBLIC
        ${PROJECT_NAME}_Common_Database_Interface
        ${PROJECT_NAME}_Common_Database_PqxxClient
        ${PROJECT_NAME}_Common_Database_MockClient
)
target_include_directories(${PROJECT_NAME}_Common_Database_Factory
        PUBLIC
        factory/
)
##############################################################################