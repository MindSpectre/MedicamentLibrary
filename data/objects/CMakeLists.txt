# PARENT_PROJECT_NAME = Data
set(PARENT_PROJECT_NAME ${PARENT_PROJECT_NAME}.Objects)

##############################################################################
# Objects: Medicaments
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Medicaments
        medicaments/source/medicaments.cpp
)
target_link_libraries(${PARENT_PROJECT_NAME}.Medicaments PRIVATE
        DrugLib.Common.Database.Base
        JsonCpp::JsonCpp
)
target_include_directories(${PARENT_PROJECT_NAME}.Medicaments
        PUBLIC
        medicaments/include
        medicaments/
)
##############################################################################


##############################################################################
# Objects: Organizations
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Organizations
        organizations/source/organizations.cpp
)
target_link_libraries(${PARENT_PROJECT_NAME}.Organizations PRIVATE
        DrugLib.Common.Database.Base
        JsonCpp::JsonCpp
)
target_include_directories(${PARENT_PROJECT_NAME}.Organizations
        PUBLIC
        organizations/include
        organizations/
)
##############################################################################

##############################################################################
# Objects: Patients
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Patients
        patients/source/patients.cpp
)
target_link_libraries(${PARENT_PROJECT_NAME}.Patients PRIVATE
        DrugLib.Common.Database.Base
        JsonCpp::JsonCpp
)
target_include_directories(${PARENT_PROJECT_NAME}.Patients
        PUBLIC
        patients/include
        patients/
)
##############################################################################