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

add_subdirectory(cpp/source-inspector)
add_subdirectory(compiler-basics/tokenizer)

if(COMPILER_STUDY_BUILD_LLVM_PASS)
    add_subdirectory(llvm-pass)
endif()
