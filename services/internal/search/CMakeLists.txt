##############################################################################
# Services Internal: Search service
##############################################################################
add_library(DrugLib_Services_Internal_Search
        source/search_service_internal.cpp
        include/search_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_Search
        PUBLIC
        ${InternalLibs}
)
target_include_directories(DrugLib_Services_Internal_Search
        PUBLIC
        include
)
##############################################################################