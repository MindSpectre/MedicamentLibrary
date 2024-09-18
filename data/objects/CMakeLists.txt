set(PARENT_PROJECT_NAME ${PROJECT_NAME}.Objects)

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
)