# PARENT_PROJECT_NAME = NULL

set(PARENT_PROJECT_NAME ${PROJECT_NAME}.Dao)


##############################################################################
# DAO CRUD INTERFACE
##############################################################################
add_library(${PARENT_PROJECT_NAME}.CrudInterface
        INTERFACE
        crud_interface/
)

target_link_libraries(${PARENT_PROJECT_NAME}.CrudInterface
        INTERFACE
        JsonCpp::JsonCpp
        DrugLib.Common.Database.Base
)
target_include_directories(${PARENT_PROJECT_NAME}.CrudInterface
        INTERFACE
        crud_interface/
)
##############################################################################

##############################################################################

add_subdirectory(handbooks)
