cmake_minimum_required(VERSION 3.20)

project(toy_interpreter LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTERNSIONS OFF)

add_executable(toy-interpreter
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
)

target_compile_options(toy-interpreter PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

enable_testing()

add_test(
    NAME interpreter_unary_minus
    COMMAND ${CMAKE_COMMAND}
        "-DINTERPRETER=$<TARGET_FILE:toy-interpreter>"
        "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/unary-inputs"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/unary.cmake"
)

add_test(
    NAME interpreter_diagnostic_locations
    COMMAND ${CMAKE_COMMAND}
        "-DINTERPRETER=$<TARGET_FILE:toy-interpreter>"
        "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/diagnostic-inputs"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/diagnostics.cmake"
)

add_test(
    NAME interpreter_smoke
    COMMAND
        toy-interpreter
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/arithmetic.toy
)

set_tests_properties(
    interpreter_smoke
    PROPERTIES
        PASS_REGULAR_EXPRESSION "38"
)

add_test(
    NAME interpreter_rejects_missing_file
    COMMAND
        toy-interpreter
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/missing.toy
)

set_tests_properties(
    interpreter_rejects_missing_file
    PROPERTIES
        WILL_FAIL TRUE
)

add_test(
    NAME interpreter_rejects_missing_semicolon
    COMMAND
        toy-interpreter
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/invalid/missing_semicolon.toy
)

set_tests_properties(
    interpreter_rejects_missing_semicolon
    PROPERTIES
        WILL_FAIL TRUE
)

add_test(
        NAME interpreter_rejects_duplicate_variable
        COMMAND
        toy-interpreter
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/invalid/duplicate_variable.toy
)

set_tests_properties(
        interpreter_rejects_duplicate_variable
        PROPERTIES
        WILL_FAIL TRUE
)
