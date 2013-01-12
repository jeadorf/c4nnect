cmake_minimum_required(VERSION 2.8.7)

project(c4nnect)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(BUILD_SHARED_LIBS true)

include_directories("include")
include_directories("test")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# -----------------------------------------------------------------------------
# BUILD MAGIC
# -----------------------------------------------------------------------------

include_directories("${PROJECT_BINARY_DIR}/include")
configure_file(
    "${PROJECT_SOURCE_DIR}/include/configure.h.in"
    "${PROJECT_BINARY_DIR}/include/configure.h")

# -----------------------------------------------------------------------------
# LIBRARIES
# -----------------------------------------------------------------------------

file(GLOB srcs src/*.c)
add_library(${PROJECT_NAME} ${srcs})

file(GLOB tests test/*.c)
add_library(${PROJECT_NAME}_test ${tests})

# -----------------------------------------------------------------------------
# EXECUTABLES
# -----------------------------------------------------------------------------

add_executable(${PROJECT_NAME}_exe main.c)
target_link_libraries(${PROJECT_NAME}_exe ${PROJECT_NAME})
# This is a small trick that allows us to have the library and the executable
# share the same name. It is quite intuitive to name the library after the
# project, and the executable as well.
set_target_properties(${PROJECT_NAME}_exe PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Copy the test data to the project binary directory. The location of this
# directory can be accessed from within the program by including a specially
# configured header.
file(COPY "${PROJECT_SOURCE_DIR}/data"
    DESTINATION "${PROJECT_BINARY_DIR}")

# -----------------------------------------------------------------------------
# TESTS
# -----------------------------------------------------------------------------

enable_testing()

add_executable(test test/test.c)
target_link_libraries(test ${PROJECT_NAME})
target_link_libraries(test ${PROJECT_NAME}_test)
add_test(test test)
