# DATA_OBJECTS = Data
add_library(DrugLib_Data_Objects
        base/source/objects.cpp
        base/include/objects.hpp
)
target_link_libraries(DrugLib_Data_Objects
        PUBLIC
        DrugLib_Data_PropertyFactory
)
target_include_directories(DrugLib_Data_Objects
        PUBLIC
        base/include
        base/
)
add_subdirectory(properties)