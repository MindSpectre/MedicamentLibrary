# Define the executable
add_executable(DrugLib_Services_Drogon_Search
        main.cpp
        source/search_service.cpp
)

# Include directories
target_include_directories(DrugLib_Services_Drogon_Search
        PRIVATE
        controller
        include
)

# Link Drogon library
target_link_libraries(DrugLib_Services_Drogon_Search
        PRIVATE
        ${NecessaryDrogonLibs}
        DrugLib_Services_Internal_Search
)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})