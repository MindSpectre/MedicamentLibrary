# Define the executable
add_executable(DrugLib_Services_Drogon_LookUp
        main.cpp
        source/look_up_service.cpp
)

# Include directories
target_include_directories(DrugLib_Services_Drogon_LookUp
        PRIVATE
        controller
        include
)

# Link Drogon library
target_link_libraries(DrugLib_Services_Drogon_LookUp
        PRIVATE
        Drogon::Drogon
        DrugLib_Services_Internal_LookUp
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/drogon_config.json
        ${CMAKE_CURRENT_BINARY_DIR}/config/drogon_config.json
        COPYONLY
)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/params.json
        ${CMAKE_CURRENT_BINARY_DIR}/config/params.json
        COPYONLY
)