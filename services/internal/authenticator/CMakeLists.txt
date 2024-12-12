##############################################################################
# Services Internal: Authenticator service
##############################################################################
add_library(DrugLib_Services_Internal_Authenticator
        source/authenticator_service_internal.cpp
        include/authenticator_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_Authenticator
        PUBLIC
        ${InternalLibs}
)
target_include_directories(DrugLib_Services_Internal_Authenticator
        PUBLIC
        include
)
##############################################################################