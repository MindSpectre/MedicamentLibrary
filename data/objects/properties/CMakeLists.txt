set(DATA_OBJECTS_PROPERTIES DrugLib_Data_Objects_Properties)

##############################################################################
# Objects: Medicaments properties
##############################################################################
add_library(${DATA_OBJECTS_PROPERTIES}_Medicaments
        medicaments/source/medicament_properties.cpp
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
        ${CMAKE_SOURCE_DIR}/objects/base/include
)

##############################################################################


##############################################################################
# Objects: Organizations properties
##############################################################################
add_library(${DATA_OBJECTS_PROPERTIES}_Organizations
        organizations/source/organization_properties.cpp
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
# Objects: Patients properties
##############################################################################
add_library(${DATA_OBJECTS_PROPERTIES}_Patients
        patients/source/patient_properties.cpp
        data_property.hpp
)
target_link_libraries(${DATA_OBJECTS_PROPERTIES}_Patients PUBLIC
        DrugLib_Common_Database_Base
        JsonCpp::JsonCpp
)
target_include_directories(${DATA_OBJECTS_PROPERTIES}_Patients
        PUBLIC
        patients/include
        patients/
        ${CMAKE_CURRENT_SOURCE_DIR}
)
##############################################################################