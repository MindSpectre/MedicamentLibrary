# PARENT_PROJECT_NAME = NULL

set(DATA_LIBS ${PROJECT_NAME}_Data)

##############################################################################

add_subdirectory(objects)

add_library(${DATA_LIBS}_Base
        INTERFACE
        base/data_property.hpp
        creational/property_factory/property_factory.hpp
)
target_link_libraries(${DATA_LIBS}_Base
        INTERFACE
        JsonCpp::JsonCpp
        Boost::container

)
target_include_directories(${DATA_LIBS}_Base
        INTERFACE
        base
)

add_library(${DATA_LIBS}_PropertyFactory
        STATIC
        creational/property_factory/property_factory.cpp
)
target_link_libraries(${DATA_LIBS}_PropertyFactory
        PUBLIC
        ${DATA_LIBS}_Base
        ${DATA_LIBS}_Objects_Medicaments
        ${DATA_LIBS}_Objects_Organizations
        ${DATA_LIBS}_Objects_Patients
)
target_include_directories(${DATA_LIBS}_PropertyFactory
        PUBLIC
        creational/property_factory

)
add_library(${DATA_LIBS}_Objects
        INTERFACE
        objects/base/medicament_base.hpp
        objects/base/organization_base.hpp
        objects/base/patients_base.hpp
        objects/base/common_obj.hpp
)
target_link_libraries(${DATA_LIBS}_Objects
        INTERFACE
        ${DATA_LIBS}_PropertyFactory
)