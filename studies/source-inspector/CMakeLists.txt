cmake_minimum_required(VERSION 3.20)
project(source_inspector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(source-inspector
    main.cpp
    application.cpp
    source_analyzer.cpp
    report_printer.cpp
)

target_compile_options(source-inspector PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

enable_testing()

add_test(
    NAME analyze_existing_file
    COMMAND source-inspector ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_test(
    NAME reject_missing_file
    COMMAND source-inspector ${CMAKE_CURRENT_SOURCE_DIR}/missing.cpp
  )
add_test(
    NAME analyze_sample_file
    COMMAND source-inspector
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/sample.cpp
)
add_test(
    NAME analyze_empty_file
    COMMAND source-inspector
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/empty.cpp
)
add_test(
    NAME analyze_multiple_files
    COMMAND source-inspector
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/only_comments.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/sample.cpp
  )


set_tests_properties(
    reject_missing_file
    PROPERTIES WILL_FAIL TRUE
)

set_tests_properties(
    analyze_sample_file
    PROPERTIES PASS_REGULAR_EXPRESSION "Total Lines: 4"
)

set_tests_properties(
    analyze_empty_file
    PROPERTIES PASS_REGULAR_EXPRESSION "Total Lines: 0"
)
