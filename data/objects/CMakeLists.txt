# PARENT_PROJECT_NAME = NULL
set(PARENT_PROJECT_NAME ${PROJECT_NAME}.Objects)

##############################################################################
# Objects: Medicaments
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Meds
    medicaments/source/medicaments.cpp
)
target_link_libraries(${PARENT_PROJECT_NAME}.Meds PRIVATE
    DrugLib.Common.Database.Base
    JsonCpp::JsonCpp
)
target_include_directories(${PARENT_PROJECT_NAME}.Meds
    PUBLIC
    medicaments/include
    medicaments/
)
##############################################################################


##############################################################################
# Objects: Organizations
##############################################################################
add_library(${PARENT_PROJECT_NAME}.Organizations
    organizations/source/organizations.cpp
)
target_link_libraries(${PARENT_PROJECT_NAME}.Organizations PRIVATE
    DrugLib.Common.Database.Base
    JsonCpp::JsonCpp
)
target_include_directories(${PARENT_PROJECT_NAME}.Organizations
    PUBLIC
    organizations/include
    organizations/
)
##############################################################################