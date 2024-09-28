# COMMON_PREFIX = Data
set(COMMON_PREFIX DrugLib_Data_Objects)

##############################################################################
# Objects: Medicaments
##############################################################################
add_library(${COMMON_PREFIX}_Medicaments
        medicaments/source/medicaments.cpp
)
target_link_libraries(${COMMON_PREFIX}_Medicaments PRIVATE
        DrugLib_Common_Database_Base
        ${DATA_LIBS}_Base
        JsonCpp::JsonCpp
)
target_include_directories(${COMMON_PREFIX}_Medicaments
        PUBLIC
        medicaments/include
        medicaments/
)
##############################################################################


##############################################################################
# Objects: Organizations
##############################################################################
add_library(${COMMON_PREFIX}_Organizations
        organizations/source/organizations.cpp
)
target_link_libraries(${COMMON_PREFIX}_Organizations PRIVATE
        DrugLib_Common_Database_Base
        JsonCpp::JsonCpp
)
target_include_directories(${COMMON_PREFIX}_Organizations
        PUBLIC
        organizations/include
        organizations/
)
##############################################################################

##############################################################################
# Objects: Patients
##############################################################################
add_library(${COMMON_PREFIX}_Patients
        patients/source/patients.cpp
)
target_link_libraries(${COMMON_PREFIX}_Patients PRIVATE
        DrugLib_Common_Database_Base
        JsonCpp::JsonCpp
)
target_include_directories(${COMMON_PREFIX}_Patients
        PUBLIC
        patients/include
        patients/
)
##############################################################################