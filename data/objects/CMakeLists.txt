# DATA_OBJECTS = Data
set(DATA_OBJECTS DrugLib_Data_Objects)

##############################################################################
# Objects: Medicaments
##############################################################################
add_library(${DATA_OBJECTS}_Medicaments
        properties/medicaments/source/medicaments.cpp
)
target_link_libraries(${DATA_OBJECTS}_Medicaments PUBLIC
        DrugLib_Common_Database_Base
        ${DATA_LIBS}_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS}_Medicaments
        PUBLIC
        properties/medicaments/include
        properties/medicaments/
)

##############################################################################


##############################################################################
# Objects: Organizations
##############################################################################
add_library(${DATA_OBJECTS}_Organizations
        properties/organizations/source/organizations.cpp
)
target_link_libraries(${DATA_OBJECTS}_Organizations PUBLIC
        DrugLib_Common_Database_Base
        ${DATA_LIBS}_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS}_Organizations
        PUBLIC
        properties/organizations/include
        properties/organizations/
)
##############################################################################

##############################################################################
# Objects: Patients
##############################################################################
add_library(${DATA_OBJECTS}_Patients
        properties/patients/source/patients.cpp
)
target_link_libraries(${DATA_OBJECTS}_Patients PUBLIC
        DrugLib_Common_Database_Base
        ${DATA_LIBS}_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS}_Patients
        PUBLIC
        properties/patients/include
        properties/patients/
)
##############################################################################