# Define paths
#set(CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}")
#set(SOURCE_CONFIG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/config")
#
## List files to move (wildcard is not directly supported, so list explicitly or use generator expressions)
#file(GLOB CONFIG_FILES "${CONFIG_DIR}/*.json")
#
## Add a custom command to move files
#add_custom_command(
#        OUTPUT move_files
#        COMMAND ${CMAKE_COMMAND} -E make_directory "${SOURCE_CONFIG_DIR}" # Ensure the source directory exists
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CONFIG_FILES} "${SOURCE_CONFIG_DIR}"
#        COMMAND ${CMAKE_COMMAND} -E rm ${CONFIG_FILES} # Optional: Remove files from the binary dir after copying
#)
#
## Add a custom target that depends on the custom command
#add_custom_target(move_config_files ALL
#        DEPENDS move_files
#)
#
## Optionally, add a dependency to ensure this runs after build
#add_dependencies(move_config_files DrugLib_Tools_JsonCreationFromMockObjects)
add_executable(DrugLib_Tools_JsonCreationFromMockObjects
    json_creation_from_mock_objects.cpp
)

target_link_libraries(DrugLib_Tools_JsonCreationFromMockObjects
    PUBLIC
        DrugLib_Data_Objects
)