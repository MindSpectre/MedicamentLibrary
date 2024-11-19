##############################################################################
# Services Internal: Look up service
##############################################################################
add_library(DrugLib_Services_Internal_LookUp
        source/lookup_service_internal.cpp
        include/lookup_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_LookUp
        PUBLIC
        DrugLib_Dao_Handbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_HashCreator_Factory
)
target_include_directories(DrugLib_Services_Internal_LookUp
        PUBLIC
        include
)
##############################################################################