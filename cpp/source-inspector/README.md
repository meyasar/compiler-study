# Source Inspector

## Overview

Source Inspector is a C++17 command-line application that reports basic physical line metrics for C and C++ source files.

## Features

- Analyzes one or more files
- Reports total, blank, and non-blank code lines
- Reports missing files and returns a nonzero exit code

## Build

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

## Usage

```bash
./build/source-inspector main.cpp
./build/source-inspector main.cpp application.cpp
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```
