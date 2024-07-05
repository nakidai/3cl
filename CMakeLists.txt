cmake_minimum_required(VERSION 3.5)
project(3cl VERSION 0.1 LANGUAGES C)

if (PROJECT_SOURCE_DIR STREQUAL PORJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

add_executable(
    "${PROJECT_NAME}"
    src/cccl.c
    src/main.c
    src/utils.c
    src/platform/getch.c
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set_target_properties("${PROJECT_NAME}" PROPERTIES C_STANDARD 11)
set_target_properties("${PROJECT_NAME}" PROPERTIES C_EXTENSIONS FALSE)

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
