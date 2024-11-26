##############################################################################
# Services Internal: Authenticator service
##############################################################################
add_library(DrugLib_Services_Internal_Authenticator
        source/authenticator_service_internal.cpp
        include/authenticator_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_Authenticator
        PUBLIC
        DrugLib_Dao_SuperHandbook
        DrugLib_Common_Database_Factory
        DrugLib_Common_HashCreator_Factory
)
target_include_directories(DrugLib_Services_Internal_Authenticator
        PUBLIC
        include
)
##############################################################################