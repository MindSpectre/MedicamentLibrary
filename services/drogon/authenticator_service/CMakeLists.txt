# Define the executable
add_executable(DrugLib_Services_Drogon_Authenticator
        main.cpp
        source/authenticator.cpp
)

# Include directories
target_include_directories(DrugLib_Services_Drogon_Authenticator PRIVATE controller include)

# Link Drogon library
target_link_libraries(DrugLib_Services_Drogon_Authenticator
        PRIVATE
        DrugLib_Services_Internal_Authenticator
        ${NecessaryDrogonLibs}
)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
