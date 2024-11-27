##############################################################################
# Services Internal: Patient Info Editor
##############################################################################
add_library(DrugLib_Services_Internal_PatientInfoEditor
        source/patient_info_editor_service_internal.cpp
        include/patient_info_editor_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_PatientInfoEditor
        PUBLIC
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_HashCreator_Factory
)
target_include_directories(DrugLib_Services_Internal_PatientInfoEditor
        PUBLIC
        include
)
##############################################################################