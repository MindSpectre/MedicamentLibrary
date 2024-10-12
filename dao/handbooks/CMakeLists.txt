# PARENT_PROJECT_NAME = DrugLib.Dao

set(Handbooks ${DAL}_Handbook)

##############################################################################
# Handbook: Medicament
##############################################################################

add_library(${Handbooks}_Medicaments
        STATIC
        medicaments/source/medicaments_handbook.cpp
)

target_link_libraries(${Handbooks}_Medicaments
        PRIVATE
        JsonCpp::JsonCpp
        DrugLib_Dao_CrudInterface
        DrugLib_Common_Database_PqxxClient
        DrugLib_Data_Objects
)
target_include_directories(${Handbooks}_Medicaments
        PUBLIC
        medicaments/include
)
##############################################################################

##############################################################################
# Handbook: Organization
##############################################################################

add_library(${Handbooks}_Organizations
        STATIC
        organizations/source/organizations_handbook.cpp
)

target_link_libraries(${Handbooks}_Organizations
        PRIVATE
        JsonCpp::JsonCpp
        DrugLib_Dao_CrudInterface
        DrugLib_Data_Objects_Organizations
)
target_include_directories(${Handbooks}_Organizations
        PUBLIC
        organizations/include
)
##############################################################################

##############################################################################
# Handbook: Medicament
##############################################################################

add_library(${Handbooks}_Patients
        STATIC
        patients/source/patients_handbook.cpp
)

target_link_libraries(${Handbooks}_Patients
        PRIVATE
        JsonCpp::JsonCpp
        DrugLib_Dao_CrudInterface
        DrugLib_Data_Objects_Patients
)
target_include_directories(${Handbooks}_Patients
        PUBLIC
        patients/include
)
##############################################################################