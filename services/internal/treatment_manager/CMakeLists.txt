##############################################################################
# Services Internal: Treatment Manager service
##############################################################################
add_library(DrugLib_Services_Internal_TreatmentManager
        source/treatment_manager_service_internal.cpp
        include/treatment_manager_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_TreatmentManager
        PUBLIC
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_HashCreator_Factory
)
target_include_directories(DrugLib_Services_Internal_TreatmentManager
        PUBLIC
        include
)
##############################################################################