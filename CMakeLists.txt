cmake_minimum_required(VERSION 3.5)
project(3cl VERSION 0.1 LANGUAGES C)

if (PROJECT_SOURCE_DIR STREQUAL PORJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed")
endif()

add_executable(
    "${PROJECT_NAME}"
    src/3cl.c
    src/instruction.c
    src/instruction/nop.c
    src/instruction/pushzero.c
    src/instruction/increment.c
    src/instruction/decrement.c
    src/instruction/add.c
    src/instruction/subtract.c
    src/instruction/reverse.c
    src/instruction/assign.c
    src/instruction/invalid.c
    src/main.c
    src/platform/getch.c
    src/readchar.c
    src/readfile.c
    src/stack.c
    src/utils.c
    src/variable.c
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

set_target_properties("${PROJECT_NAME}" PROPERTIES C_STANDARD 11)
set_target_properties("${PROJECT_NAME}" PROPERTIES C_EXTENSIONS FALSE)

target_include_directories("${PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
