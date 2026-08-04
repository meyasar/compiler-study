# Source Inspector

## Overview

Source Inspector is a C++17 command-line application that reports basic physical line metrics for one or more C and C++ source files. It was built for me to learn C++ basics.

## Features

- Analyzes one or more files in a single invocation
- Reports total, blank, and non-blank code lines for each file
- Continues analyzing valid files when another input is missing
- Returns a nonzero exit code if any input cannot be opened

## Requirements

- A C++17-compatible compiler
- CMake 3.20 or later
- Ninja

## Build

```bash
cmake -S . -B build -G Ninja
cmake --build build
```

To explicitly select the `clang++` available in the shell, use:

```bash
CXX=clang++ cmake --fresh -S . -B build -G Ninja
cmake --build build
```

## Usage

Analyze one file:

```bash
./build/source-inspector main.cpp
```

Analyze multiple files:

```bash
./build/source-inspector main.cpp application.cpp
```

The report contains the total number of physical lines and their blank/non-blank classification.

## Architecture

```text
CLI arguments
    ↓
Application
    ↓
SourceAnalyzer
    ↓
AnalysisResult
    ↓
ReportPrinter
    ↓
TextReportPrinter
```

- `Application` validates arguments and coordinates multi-file analysis.
- `SourceAnalyzer` reads and classifies physical lines.
- `AnalysisResult` stores the collected metrics.
- `ReportPrinter` defines the polymorphic reporting interface.
- `TextReportPrinter` writes the current text report.

## Tests

```bash
ctest --test-dir build --output-on-failure
```

The CTest suite covers an existing file, a missing file, a sample file, an empty file, and multiple input files.

## Current Limitations

- Comment-only lines are counted as code lines.
- The application does not tokenize or parse C++ syntax.
- Directory traversal is not supported.
- Metrics are based on physical lines, not logical statements.
