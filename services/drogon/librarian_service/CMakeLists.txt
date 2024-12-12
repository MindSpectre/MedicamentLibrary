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
        ${NecessaryDrogonLibs}
        DrugLib_Services_Internal_Librarian
)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})