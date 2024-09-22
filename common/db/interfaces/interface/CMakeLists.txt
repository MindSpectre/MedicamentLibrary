# PARENT_PROJECT_NAME = DrugLib.Common.Database
set(TARGET ${PARENT_PROJECT_NAME}.DBI)


##############################################################################
# Database Client Interface
##############################################################################
add_library(${TARGET}
        INTERFACE
        db_interface.hpp
)

target_link_libraries(${TARGET}
        INTERFACE
        JsonCpp::JsonCpp
        DrugLib.Common.Database.Base
)
target_include_directories(${TARGET}
        INTERFACE
        interface
)
##############################################################################