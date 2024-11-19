##############################################################################
# Database interface Factory class
##############################################################################

add_library(DrugLib_Common_Database_Factory
        STATIC
        factory/db_interface_factory.cpp
        factory/db_interface_factory.hpp
)
target_link_libraries(DrugLib_Common_Database_Factory
        PUBLIC
        DrugLib_Common_Database_Interface
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_MockClient
)
target_include_directories(DrugLib_Common_Database_Factory
        PUBLIC
        factory/
)
##############################################################################


##############################################################################
# Database interface Pool class
##############################################################################

add_library(DrugLib_Common_Database_Pool
        STATIC
        pool/db_interface_pool.cpp
        pool/db_interface_pool.hpp
)
target_link_libraries(DrugLib_Common_Database_Pool
        PUBLIC
        DrugLib_Common_Database_Interface
)
target_include_directories(DrugLib_Common_Database_Pool
        PUBLIC
        pool/
)
##############################################################################