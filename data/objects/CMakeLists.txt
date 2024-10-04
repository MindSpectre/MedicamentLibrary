##############################################################################
# Objects: Final usable objects
##############################################################################
add_library(DrugLib_Data_Objects
        base/source/objects.cpp
        base/include/objects.hpp
)
target_link_libraries(DrugLib_Data_Objects
        PRIVATE
        DrugLib_Data_PropertyFactory
)
target_include_directories(DrugLib_Data_Objects
        PRIVATE
        base/include
        base/
)
##############################################################################
add_subdirectory(properties)