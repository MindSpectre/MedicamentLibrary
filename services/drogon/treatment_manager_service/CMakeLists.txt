
add_executable(DrugLib_Services_Drogon_TreatmentManager
        main.cpp
        source/treatment_manager_service.cpp
)

target_include_directories(DrugLib_Services_Drogon_TreatmentManager
        PRIVATE
        controller
        include
)

target_link_libraries(DrugLib_Services_Drogon_TreatmentManager
        PRIVATE
        ${NecessaryDrogonLibs}
)

file(COPY config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
