##############################################################################
# Data: Behavioral Property factory
##############################################################################
add_library(DrugLib_Data_PropertiesController
        property_factory/source/properties_controller.cpp
)
target_link_libraries(DrugLib_Data_PropertiesController
        PUBLIC
        DrugLib_Data_Objects_Properties_Medicaments
        DrugLib_Data_Objects_Properties_Organizations
        DrugLib_Data_Objects_Properties_Patients
        DrugLib_Data_Objects_Properties_Diseases
)
target_include_directories(DrugLib_Data_PropertiesController
        PUBLIC
        property_factory/include
)
##############################################################################