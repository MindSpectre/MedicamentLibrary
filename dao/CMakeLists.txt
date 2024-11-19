##############################################################################
# DAO CRUD INTERFACE
##############################################################################
add_library(DrugLib_Dao_Handbook_Base
        INTERFACE
        handbook_base/handbook_base.hpp
)

target_link_libraries(DrugLib_Dao_Handbook_Base
        INTERFACE
        DrugLib_Common_Database_Base
        DrugLib_Common_Database_PqxxClient
        DrugLib_Common_Database_Factory
        DrugLib_Common_Database_Pool
        DrugLib_Data_Objects
)
target_include_directories(DrugLib_Dao_Handbook_Base
        INTERFACE
        handbook_base/
)
##############################################################################

add_subdirectory(handbooks)

##############################################################################
add_library(DrugLib_Dao_Handbook STATIC
        handbooks/super_handbook.hpp)

target_link_libraries(DrugLib_Dao_Handbook
        PUBLIC
        DrugLib_Dao_Handbook_Diseases
        DrugLib_Dao_Handbook_Patients
        DrugLib_Dao_Handbook_Organizations
        DrugLib_Dao_Handbook_Medicaments
)
target_include_directories(DrugLib_Dao_Handbook
        PUBLIC
        handbooks/
)

