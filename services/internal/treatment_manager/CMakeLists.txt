##############################################################################
# Services Internal: Treatment Manager service
##############################################################################
add_library(DrugLib_Services_Internal_TreatmentManager
        source/treatment_manager_service_internal.cpp
        include/treatment_manager_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_TreatmentManager
        PUBLIC
        ${InternalLibs}
)
target_include_directories(DrugLib_Services_Internal_TreatmentManager
        PUBLIC
        include
)
##############################################################################