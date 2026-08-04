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

enable_testing()
add_test(
    NAME analyze_existing_file
    COMMAND source-inspector ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
add_test(
    NAME reject_missing_file
    COMMAND source-inspector ${CMAKE_CURRENT_SOURCE_DIR}/missing.cpp
  )
set_tests_properties(
    reject_missing_file
    PROPERTIES WILL_FAIL TRUE
)
