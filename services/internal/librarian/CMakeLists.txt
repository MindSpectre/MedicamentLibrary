##############################################################################
# Services Internal: Look up service
##############################################################################
add_library(DrugLib_Services_Internal_Librarian
        source/librarian_service_internal.cpp
        include/librarian_service_internal.hpp
)
target_link_libraries(DrugLib_Services_Internal_Librarian
        PUBLIC
        ${InternalLibs}
)
target_include_directories(DrugLib_Services_Internal_Librarian
        PUBLIC
        include
)
##############################################################################