# Define the executable
add_executable(DrugLib_Services_Drogon_Librarian
        main.cpp
        source/librarian_service.cpp

)

# Include directories
target_include_directories(DrugLib_Services_Drogon_Librarian
        PRIVATE
        controller
        include
)

# Link Drogon library
target_link_libraries(DrugLib_Services_Drogon_Librarian
        PRIVATE
        Drogon::Drogon
        DrugLib_Services_Internal_Librarian
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