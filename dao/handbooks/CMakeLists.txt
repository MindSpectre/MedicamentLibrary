# PARENT_PROJECT_NAME = DrugLib.Dao

set(PARENT_PROJECT_NAME ${PARENT_PROJECT_NAME}.Handbook)

##############################################################################
# Handbook: Medicament
##############################################################################

add_library(${PARENT_PROJECT_NAME}.Medicaments
        STATIC
        medicaments/source/medicaments_handbook.cpp
)

target_link_libraries(${PARENT_PROJECT_NAME}.Medicaments
        PRIVATE
        JsonCpp::JsonCpp
        DrugLib.Dao.CrudInterface
        DrugLib.Data.Objects.Medicaments
)
target_include_directories(${PARENT_PROJECT_NAME}.Medicaments
        PUBLIC
        medicaments/include
)
##############################################################################

##############################################################################
# Handbook: Organization
##############################################################################

add_library(${PARENT_PROJECT_NAME}.Organizations
        STATIC
        organizations/source/organizations_handbook.cpp
)

target_link_libraries(${PARENT_PROJECT_NAME}.Organizations
        PRIVATE
        JsonCpp::JsonCpp
        DrugLib.Dao.CrudInterface
        DrugLib.Data.Objects.Organizations
)
target_include_directories(${PARENT_PROJECT_NAME}.Organizations
        PUBLIC
        organizations/include
)
##############################################################################

##############################################################################
# Handbook: Medicament
##############################################################################

add_library(${PARENT_PROJECT_NAME}.Patients
        STATIC
        patients/source/patients_handbook.cpp
)

target_link_libraries(${PARENT_PROJECT_NAME}.Patients
        PRIVATE
        JsonCpp::JsonCpp
        DrugLib.Dao.CrudInterface
        DrugLib.Data.Objects.Patients
)
target_include_directories(${PARENT_PROJECT_NAME}.Patients
        PUBLIC
        patients/include
)
##############################################################################