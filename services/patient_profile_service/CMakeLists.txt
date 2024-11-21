##############################################################################
# Services Internal: Patient Profile service
##############################################################################
add_library(DrugLib_Services_Internal_PatientProfile
        source/patient_profile_service_internal.cpp
        include/patient_profile_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_PatientProfile
        PUBLIC
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_HashCreator_Factory
)
target_include_directories(DrugLib_Services_Internal_PatientProfile
        PUBLIC
        include
)
##############################################################################