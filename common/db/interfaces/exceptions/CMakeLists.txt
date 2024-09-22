# PARENT_PROJECT_NAME = DrugLib.Common.Database
set(TARGET ${PARENT_PROJECT_NAME}.Exceptions)

##############################################################################
# Error handling 
##############################################################################
add_library(${TARGET} INTERFACE)
target_include_directories(${TARGET} INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)
##############################################################################

