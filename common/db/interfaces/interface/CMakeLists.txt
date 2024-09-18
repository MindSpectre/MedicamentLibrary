set(TARGET ${PARENT_PROJECT_NAME}.DBI)

add_library(${TARGET}
    INTERFACE
    )

target_link_libraries(${TARGET}
    JsonCpp::JsonCpp
    DrugLib.Database.Base
)
target_include_directories(${TARGET}
    INTERFACE
    interface
)