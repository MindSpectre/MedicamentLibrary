# Define the executable
add_executable(DrugLib_Services_Drogon_Gateway
        main.cpp
        source/gateway.cpp
)

# Include directories
target_include_directories(DrugLib_Services_Drogon_Gateway PRIVATE controller include)

# Link Drogon library
target_link_libraries(DrugLib_Services_Drogon_Gateway
        PRIVATE
        ${NecessaryDrogonLibs}
)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
