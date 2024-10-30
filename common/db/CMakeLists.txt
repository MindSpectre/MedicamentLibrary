##############################################################################
# Database main classes NECESSARY UTILITY
##############################################################################
add_library(${PROJECT_NAME}_Common_Database_Base INTERFACE
        base/db_conditions.hpp
        base/db_field.hpp
        base/db_record.hpp
        base/db_serializer.hpp
        base/db_controller.hpp
)

target_include_directories(${PROJECT_NAME}_Common_Database_Base
        INTERFACE
        base
)
target_link_libraries(${PROJECT_NAME}_Common_Database_Base
        INTERFACE
        Boost::boost
        JsonCpp::JsonCpp
)
##############################################################################

##############################################################################
# Database Client Interface
##############################################################################
add_library(${PROJECT_NAME}_Common_Database_Interface
        INTERFACE
        interface/db_interface.hpp
)

target_link_libraries(${PROJECT_NAME}_Common_Database_Interface
        INTERFACE
        JsonCpp::JsonCpp
        ${PROJECT_NAME}_Common_Database_Base
)
target_include_directories(${PROJECT_NAME}_Common_Database_Interface
        INTERFACE
        interface
)
##############################################################################

##############################################################################
# Error handling
##############################################################################
add_library(${PROJECT_NAME}_Common_Database_Exceptions INTERFACE
        exceptions/exceptions.hpp
        exceptions/error_codes.hpp)
target_include_directories(${PROJECT_NAME}_Common_Database_Exceptions INTERFACE
        exceptions
)
##############################################################################

##############################################################################


add_subdirectory(creational)

add_subdirectory(behavioral)

add_subdirectory(postgres/pqxx_impl)


