cmake_minimum_required(VERSION 3.20)
project(tokenizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTERNSIONS OFF)

add_executable(tokenizer
    main.cpp
    lexer.cpp
    parser.cpp
    ast.cpp
)

target_compile_options(tokenizer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

enable_testing()

add_test(
    NAME interpreter_smoke
    COMMAND
        tokenizer
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
        tokenizer
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
        tokenizer
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/invalid/missing_semicolon.toy
)

set_tests_properties(
    interpreter_rejects_missing_semicolon
    PROPERTIES
        WILL_FAIL TRUE
)
