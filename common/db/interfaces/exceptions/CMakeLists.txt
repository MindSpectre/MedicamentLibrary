set(TARGET ${PARENT_PROJECT_NAME}.Exceptions)
add_library(${TARGET} INTERFACE)
target_include_directories(${TARGET} INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)
