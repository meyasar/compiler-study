# Toy Interpreter

A C++17 interpreter for a small toy language. It includes a lexer,
recursive-descent parser, AST, name analysis, source-located diagnostics, and
checked signed 32-bit arithmetic.

## Contents

- `src/`: toy language implementation and command-line entry point.
- `examples/`: valid and invalid toy programs.
- `tests/`: diagnostic and arithmetic test scripts.
- `docs/language.md`: integer semantics and error behavior.
- `studies/source-inspector/`: a separate C++ line-counting exercise.

## Build and test

The interpreter requires a C++17 compiler and CMake. For the current Ubuntu
24.04 WSL environment, build from the repository root with:

```bash
cmake -S . -B ~/build/toy-interpreter -G Ninja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_COMPILER=clang++-22
cmake --build ~/build/toy-interpreter
ctest --test-dir ~/build/toy-interpreter --output-on-failure
```

Use a build directory appropriate to your machine, preferably on the Linux
filesystem when building through WSL.

## Run

```bash
~/build/toy-interpreter/toy-interpreter examples/arithmetic.toy
```

The interpreter accepts a source file and executes it directly. In CLion, reload
the CMake project and select the `toy-interpreter` target.
