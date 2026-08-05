cmake_minimum_required(VERSION 3.20)
project(tokenizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTERNSIONS OFF)

add_executable(tokenizer
    main.cpp
    lexer.cpp
)

target_compile_options(tokenizer PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
