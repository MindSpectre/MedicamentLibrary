##############################################################################
# DAO CRUD INTERFACE
##############################################################################
add_library(DrugLib_Dao_Handbook_Base
        INTERFACE
        interface/handbook_base.hpp
)

target_link_libraries(DrugLib_Dao_Handbook_Base
        INTERFACE
        DrugLib_Data_Objects
        ${DbNecessaryLibs}

)
target_include_directories(DrugLib_Dao_Handbook_Base
        INTERFACE
        interface/
)
##############################################################################


##############################################################################
# Handbook: Medicament
##############################################################################

add_library(DrugLib_Dao_Handbook_Medicaments
        STATIC
        medicaments/source/medicaments_handbook.cpp
)

target_link_libraries(DrugLib_Dao_Handbook_Medicaments
        PUBLIC
        DrugLib_Dao_Handbook_Base
)
target_include_directories(DrugLib_Dao_Handbook_Medicaments
        PUBLIC
        medicaments/include
)
##############################################################################

##############################################################################
# Handbook: Organization
##############################################################################
add_library(DrugLib_Dao_Handbook_Organizations
        STATIC
        organizations/source/organizations_handbook.cpp
)
target_link_libraries(DrugLib_Dao_Handbook_Organizations
        PUBLIC
        DrugLib_Dao_Handbook_Base
)
target_include_directories(DrugLib_Dao_Handbook_Organizations
        PUBLIC
        organizations/include
)
##############################################################################

##############################################################################
# Handbook: Medicament
##############################################################################

add_library(DrugLib_Dao_Handbook_Patients
        STATIC
        patients/source/patients_handbook.cpp
)

target_link_libraries(DrugLib_Dao_Handbook_Patients
        PUBLIC
        DrugLib_Dao_Handbook_Base
)
target_include_directories(DrugLib_Dao_Handbook_Patients
        PUBLIC
        patients/include
)
##############################################################################


##############################################################################
# Handbook: Diseases
##############################################################################
add_library(DrugLib_Dao_Handbook_Diseases
        STATIC
        diseases/source/diseases_handbook.cpp
)

target_link_libraries(DrugLib_Dao_Handbook_Diseases
        PUBLIC
        DrugLib_Dao_Handbook_Base
)
target_include_directories(DrugLib_Dao_Handbook_Diseases
        PUBLIC
        diseases/include
)
##############################################################################


##############################################################################
add_library(DrugLib_Dao_SuperHandbook STATIC
        super_handbook.hpp)

target_link_libraries(DrugLib_Dao_SuperHandbook
        PUBLIC
        DrugLib_Dao_Handbook_Diseases
        DrugLib_Dao_Handbook_Patients
        DrugLib_Dao_Handbook_Organizations
        DrugLib_Dao_Handbook_Medicaments
)
target_include_directories(DrugLib_Dao_SuperHandbook
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
##############################################################################