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
        DrugLib_Dao_CrudInterface
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
        DrugLib_Dao_CrudInterface
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
        DrugLib_Dao_CrudInterface
)
target_include_directories(${Handbooks}_Patients
        PUBLIC
        patients/include
)
##############################################################################


##############################################################################
# Handbook: Diseases
##############################################################################

add_library(${Handbooks}_Diseases
        STATIC
        diseases/source/diseases_handbook.cpp
)

target_link_libraries(${Handbooks}_Diseases
        PRIVATE
        DrugLib_Dao_CrudInterface
)
target_include_directories(${Handbooks}_Diseases
        PUBLIC
        diseases/include
)
##############################################################################