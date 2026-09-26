cmake_minimum_required(VERSION 3.20)

project(
    compiler_study
    LANGUAGES C CXX
)

include(CTest)

option(
    COMPILER_STUDY_BUILD_LLVM_PASS
    "Build the LLVM pass plugin"
    ON
)

add_subdirectory(studies/source-inspector)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTERNSIONS OFF)

add_executable(tokenizer
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/codegen.cpp
)

find_package(LLVM REQUIRED CONFIG)
if(NOT LLVM_VERSION_MAJOR EQUAL 22)
    message(FATAL_ERROR "The frontend requires LLVM 22; found ${LLVM_PACKAGE_VERSION}")
endif()
target_include_directories(tokenizer SYSTEM PRIVATE ${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_FRONTEND_DEFINITIONS NATIVE_COMMAND "${LLVM_DEFINITIONS}")
target_compile_options(tokenizer PRIVATE ${LLVM_FRONTEND_DEFINITIONS})
if(TARGET LLVM)
    target_link_libraries(tokenizer PRIVATE LLVM)
else()
    llvm_map_components_to_libnames(LLVM_FRONTEND_LIBS Core Support)
    target_link_libraries(tokenizer PRIVATE ${LLVM_FRONTEND_LIBS})
endif()

target_compile_options(tokenizer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

enable_testing()

add_test(
    NAME llvm_codegen
    COMMAND ${CMAKE_COMMAND}
        "-DINTERPRETER=$<TARGET_FILE:tokenizer>"
        "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/codegen-inputs"
        "-DOPT=${LLVM_TOOLS_BINARY_DIR}/opt"
        "-DLLI=${LLVM_TOOLS_BINARY_DIR}/lli"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/codegen.cmake"
)

add_test(
    NAME interpreter_unary_minus
    COMMAND ${CMAKE_COMMAND}
        "-DINTERPRETER=$<TARGET_FILE:tokenizer>"
        "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/unary-inputs"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/unary.cmake"
)

add_test(
    NAME interpreter_diagnostic_locations
    COMMAND ${CMAKE_COMMAND}
        "-DINTERPRETER=$<TARGET_FILE:tokenizer>"
        "-DTEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/diagnostic-inputs"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/diagnostics.cmake"
)

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

add_test(
        NAME interpreter_rejects_duplicate_variable
        COMMAND
        tokenizer
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/invalid/duplicate_variable.toy
)

set_tests_properties(
        interpreter_rejects_duplicate_variable
        PROPERTIES
        WILL_FAIL TRUE
)


if(COMPILER_STUDY_BUILD_LLVM_PASS)
    add_subdirectory(llvm-pass)
endif()
