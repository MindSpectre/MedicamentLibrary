##############################################################################
# DAO CRUD INTERFACE
##############################################################################
add_library(DrugLib_Dao_CrudInterface
        INTERFACE
        crud_interface/handbook.hpp
)

target_link_libraries(DrugLib_Dao_CrudInterface
        INTERFACE
        JsonCpp::JsonCpp
        DrugLib_Common_Database_Base
        DrugLib_Common_Database_PqxxClient
        DrugLib_Data_Objects
)
target_include_directories(DrugLib_Dao_CrudInterface
        INTERFACE
        crud_interface/
)
##############################################################################

##############################################################################

add_subdirectory(handbooks)
