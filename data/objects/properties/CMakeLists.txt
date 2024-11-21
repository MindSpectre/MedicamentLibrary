set(DATA_OBJECTS_PROPERTIES DrugLib_Data_Objects_Properties)
##############################################################################
# Objects: Main data property class
##############################################################################
add_library(DrugLib_Data_Objects_Properties_DataProperty INTERFACE
        data_property.hpp
)
target_link_libraries(DrugLib_Data_Objects_Properties_DataProperty INTERFACE
        DrugLib_Common_Database_Base
)
target_include_directories(DrugLib_Data_Objects_Properties_DataProperty
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
##############################################################################

##############################################################################
# Objects: Medicaments properties
##############################################################################
add_library(DrugLib_Data_Objects_Properties_Medicaments
        medicaments/source/medicament_properties.cpp
        medicaments/include/medicament_properties/prescription.hpp
)
target_link_libraries(DrugLib_Data_Objects_Properties_Medicaments PUBLIC
        DrugLib_Data_Objects_Properties_DataProperty
)
target_include_directories(DrugLib_Data_Objects_Properties_Medicaments
        PUBLIC
        medicaments/include
)
##############################################################################


##############################################################################
# Objects: Organizations properties
##############################################################################
add_library(DrugLib_Data_Objects_Properties_Organizations
        organizations/source/organization_properties.cpp
        organizations/include/organization_properties/license.hpp
)
target_link_libraries(DrugLib_Data_Objects_Properties_Organizations PUBLIC
        DrugLib_Data_Objects_Properties_DataProperty
)
target_include_directories(DrugLib_Data_Objects_Properties_Organizations
        PUBLIC
        organizations/include
)
##############################################################################

##############################################################################
# Objects: Patients properties
##############################################################################
add_library(DrugLib_Data_Objects_Properties_Patients
        patients/source/patient_properties.cpp
        patients/include/patient_properties/current_diseases.hpp
        patients/include/patient_properties/current_medicaments.hpp
)
target_link_libraries(DrugLib_Data_Objects_Properties_Patients PUBLIC
        DrugLib_Data_Objects_Properties_DataProperty
)
target_include_directories(DrugLib_Data_Objects_Properties_Patients
        PUBLIC
        patients/include
)
##############################################################################


##############################################################################
# Objects: Diseases properties
##############################################################################
add_library(DrugLib_Data_Objects_Properties_Diseases
        diseases/source/diseases_properties.cpp
        diseases/include/disease_properties/symptoms.hpp
        diseases/include/disease_properties/curative_drugs.hpp
)
target_link_libraries(DrugLib_Data_Objects_Properties_Diseases PUBLIC
        DrugLib_Data_Objects_Properties_DataProperty
)
target_include_directories(DrugLib_Data_Objects_Properties_Diseases
        PUBLIC
        diseases/include
)
##############################################################################