cmake_minimum_required(VERSION 2.8.7)

project(c4nnect)

# Enable for debugging
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUG")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
# The semantics are not clear, UCI table helps predicting the winner but does
# not help finding the best moves.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDISABLE_UCITABLE")

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
set(PROJECT_LIB ${PROJECT_NAME})
add_library(${PROJECT_LIB} ${srcs})

# file(GLOB tests test/*.c)
# set(PROJECT_LIB_TEST ${PROJECT_NAME}_test)
# add_library(${PROJECT_LIB_TEST} ${tests})

# -----------------------------------------------------------------------------
# EXECUTABLES
# -----------------------------------------------------------------------------

add_executable(${PROJECT_NAME}_exe main.c)
target_link_libraries(${PROJECT_NAME}_exe ${PROJECT_LIB})
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

macro(add_utest NAME)
    set(TEST_NAME ${NAME}_test)
    add_executable(${TEST_NAME} test/${TEST_NAME}.c)
    target_link_libraries(${TEST_NAME} ${PROJECT_LIB})
    # target_link_libraries(${TEST_NAME} ${PROJECT_LIB_TEST})
    add_test(${NAME} ${TEST_NAME})
endmacro()

add_utest(board)
add_utest(eval)
add_utest(parser)
add_utest(search)
add_utest(stats)
add_utest(ucitable)
