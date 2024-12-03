# Define the executable
add_executable(DrugLib_Services_Drogon_Gateway
        main.cpp
        source/gateway.cpp
)

# Include directories
target_include_directories(DrugLib_Services_Drogon_Gateway PRIVATE controller include)

# Link Drogon library
target_link_libraries(DrugLib_Services_Drogon_Gateway Drogon::Drogon JsonCpp::JsonCpp)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/drogon_config.json
        ${CMAKE_CURRENT_BINARY_DIR}/config/drogon_config.json
        COPYONLY
)