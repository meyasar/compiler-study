# Compiler Study

A C++17 toy-language interpreter and LLVM learning project. The interpreter
includes a lexer, recursive-descent parser, AST, name analysis, source-located
diagnostics, and checked signed 32-bit arithmetic. An initial LLVM backend emits
verified IR for numeric-literal print statements.

## Contents

- `src/`: toy language implementation and command-line entry point.
- `examples/`: valid and invalid toy programs.
- `tests/`: diagnostic and arithmetic test scripts.
- `docs/language.md`: integer semantics and error behavior.
- `llvm-pass/`: LLVM plugin reporting function block and instruction counts.
- `studies/source-inspector/`: independent C++ line-counting exercise.
- `studies/llvm-ir/`: C and LLVM IR learning examples.

## Build and test

The current development environment is Ubuntu 24.04 on WSL2 with Clang/LLVM
22.1.8, CMake, and Ninja. From the repository root:

```bash
cmake -S . -B /home/meren/builds/compiler-proje-debug -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=clang-22 \
  -DCMAKE_CXX_COMPILER=clang++-22 \
  -DLLVM_DIR=/usr/lib/llvm-22/lib/cmake/llvm
cmake --build /home/meren/builds/compiler-proje-debug
ctest --test-dir /home/meren/builds/compiler-proje-debug --output-on-failure
```

Use a build directory appropriate to your machine, preferably on the Linux
filesystem when building through WSL. To build without the LLVM plugin, pass
`-DCOMPILER_STUDY_BUILD_LLVM_PASS=OFF`. LLVM 22 development libraries are still
required by the frontend's IR generator.

## Run

```bash
/home/meren/builds/compiler-proje-debug/tokenizer examples/arithmetic.toy
```

The executable retains its existing name, `tokenizer`, although it now runs the
full interpreter. In CLion, keep the WSL toolchain and reload CMake after the
directory reorganization. Update program arguments to use `examples/...`.

## Initial LLVM backend

```bash
/home/meren/builds/compiler-proje-debug/tokenizer --emit-ir examples/print_number.toy > /tmp/print_number.ll
opt-22 -passes=verify -disable-output /tmp/print_number.ll
lli-22 /tmp/print_number.ll
```

`--emit-ir` parses and analyzes the entire program, constructs a module with an
`i32 main()` function, verifies it, and writes IR to stdout only on success.
Printing uses the host C runtime's `printf`. This initial backend targets the
current WSL/Linux execution environment; it does not configure cross-compilation.

Only number AST nodes and print statements are supported, including parentheses,
multiple prints, and empty programs. The parser's special minimum-int32 literal
is also a number node. Ordinary unary minus, binary arithmetic, and declarations
are rejected explicitly; their existing interpreter behavior is unchanged.
Tests verify IR using `opt`, run it using `lli`, compare output with the interpreter,
and check that rejected programs emit no partial IR.
