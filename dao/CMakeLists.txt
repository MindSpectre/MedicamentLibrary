# PARENT_PROJECT_NAME = NULL

set(DAL ${PROJECT_NAME}_Dao)


##############################################################################
# DAO CRUD INTERFACE
##############################################################################
add_library(${DAL}_CrudInterface
        INTERFACE
        crud_interface/handbook.hpp
)

target_link_libraries(${DAL}_CrudInterface
        INTERFACE
        JsonCpp::JsonCpp
        DrugLib_Common_Database_Base
        DrugLib_Common_Database_PqxxClient
        DrugLib_Data_Objects
)
target_include_directories(${DAL}_CrudInterface
        INTERFACE
        crud_interface/
)
##############################################################################

##############################################################################

add_subdirectory(handbooks)
