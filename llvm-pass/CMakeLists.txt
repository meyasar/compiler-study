cmake_minimum_required(VERSION 3.20)

project(LLVMStaticAnalyzer LANGUAGES C CXX)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake from: ${LLVM_DIR}")

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_llvm_pass_plugin(StaticAnalyzerPass src/StaticAnalyzer.cpp)

target_compile_features(StaticAnalyzerPass PRIVATE cxx_std_17)
