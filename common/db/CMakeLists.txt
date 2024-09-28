# DATABASE_LIBS = DrugLib.Common

set(DATABASE_LIBS ${COMMON_LIBS}_Database)

##############################################################################
# Database main classes NECESSARY UTILITY
##############################################################################
add_library(${DATABASE_LIBS}_Base INTERFACE)

target_include_directories(${DATABASE_LIBS}_Base
        INTERFACE
        base
)
target_link_libraries(${DATABASE_LIBS}_Base
        INTERFACE
        Boost::boost
)
##############################################################################

##############################################################################
# Database Client Interface
##############################################################################
add_library(${DATABASE_LIBS}_Interface
        INTERFACE
        interface/db_interface.hpp
)

target_link_libraries(${DATABASE_LIBS}_Interface
        INTERFACE
        JsonCpp::JsonCpp
        ${DATABASE_LIBS}_Base
)
target_include_directories(${DATABASE_LIBS}_Interface
        INTERFACE
        interface
)
##############################################################################

##############################################################################
# Error handling
##############################################################################
add_library(${DATABASE_LIBS}_Exceptions INTERFACE
        exceptions/exceptions.hpp
        exceptions/error_codes.hpp)
target_include_directories(${DATABASE_LIBS}_Exceptions INTERFACE
        exceptions
)
##############################################################################

##############################################################################


add_subdirectory(creational)

add_subdirectory(behavioral)

add_subdirectory(postgres/pqxx_impl)


