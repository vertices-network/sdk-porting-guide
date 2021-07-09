set(PROJECT_NAME myproject)
project(${PROJECT_NAME})

# add source file related to that project
set(SRC_FILES
        main.c
        http_template.c
        )

# Add directories to include below
set(INC_FOLDERS
        )

include_directories(${INC_FOLDERS})
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link with the Vertices SDK

target_link_libraries(${PROJECT_NAME} vertices)