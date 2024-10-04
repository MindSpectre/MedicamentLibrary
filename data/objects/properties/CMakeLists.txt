set(DATA_OBJECTS_PROPERTIES DrugLib_Data_Objects_Properties)

##############################################################################
# Objects: Medicaments
##############################################################################
add_library(${DATA_OBJECTS_PROPERTIES}_Medicaments
        medicaments/source/medicaments.cpp
        data_property.hpp
)
target_link_libraries(${DATA_OBJECTS_PROPERTIES}_Medicaments PUBLIC
        DrugLib_Common_Database_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS_PROPERTIES}_Medicaments
        PUBLIC
        medicaments/include
        medicaments/
        ${CMAKE_CURRENT_SOURCE_DIR}
)

##############################################################################


##############################################################################
# Objects: Organizations
##############################################################################
add_library(${DATA_OBJECTS_PROPERTIES}_Organizations
        organizations/source/organizations.cpp
        data_property.hpp
)
target_link_libraries(${DATA_OBJECTS_PROPERTIES}_Organizations PUBLIC
        DrugLib_Common_Database_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS_PROPERTIES}_Organizations
        PUBLIC
        organizations/include
        organizations/
        ${CMAKE_CURRENT_SOURCE_DIR}
)
##############################################################################

##############################################################################
# Objects: Patients
##############################################################################
add_library(${DATA_OBJECTS}_Patients
        patients/source/patients.cpp
        data_property.hpp
)
target_link_libraries(${DATA_OBJECTS}_Patients PUBLIC
        DrugLib_Common_Database_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS}_Patients
        PUBLIC
        patients/include
        patients/
        ${CMAKE_CURRENT_SOURCE_DIR}
)
##############################################################################