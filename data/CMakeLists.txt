# PARENT_PROJECT_NAME = NULL

set(DATA_LIBS ${PROJECT_NAME}_Data)

##############################################################################

add_subdirectory(objects)

add_library(${DATA_LIBS}_Base
        INTERFACE
        base
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