cmake_minimum_required(VERSION 3.20)

project(LLVMStaticAnalyzer LANGUAGES C CXX)

include(CTest)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake from: ${LLVM_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_llvm_pass_plugin(StaticAnalyzerPass src/StaticAnalyzer.cpp)

target_compile_features(StaticAnalyzerPass PRIVATE cxx_std_17)

if(BUILD_TESTING)
    add_test(
        NAME llvm_static_analyzer_smoke
        COMMAND
            ${LLVM_TOOLS_BINARY_DIR}/opt
            -load-pass-plugin=$<TARGET_FILE:StaticAnalyzerPass>
            -passes=static-analyzer
            -disable-output
            ${CMAKE_CURRENT_SOURCE_DIR}/../studies/llvm-ir/examples/arithmetic_O0.ll
    )

    set_tests_properties(
        llvm_static_analyzer_smoke
        PROPERTIES
            PASS_REGULAR_EXPRESSION "Function: compute"
    )
endif()
