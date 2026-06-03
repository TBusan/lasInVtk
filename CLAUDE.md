# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

Uses MinGW-w64 + CMake on Windows.

```bash
# Configure (first time or after CMakeLists.txt changes)
cmake -B build -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug

# Build
cmake --build build

# Run demo
./build/app/gpr_demo testData/华明路_001_A01
```

If `build/` has stale cache from a different project, delete it and reconfigure:
```bash
rm -rf build
cmake -B build -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Debug
```

## Architecture

GPR (Ground Penetrating Radar) data parser for MALA/Impulse Radar `.head`/`.data`/`.cor` file triplets.

**Data flow:** A base path (e.g. `testData/华明路_001_A01`) is extended with `.head`, `.data`, `.cor` suffixes. Files are parsed in order: head (metadata needed by other parsers) -> cor (coordinates) -> data (trace samples).

**Library (`src/`):** Static library `gpr_parser` with three independent parsers plus a facade:
- `GprHeadParser` — parses `.head` key-value text file into `GprMetadata` struct. Skips `#` comment lines and inline comments. The `DATA VERSION` field (16 or 32) determines bytes per sample.
- `GprCorParser` — parses `.cor` whitespace-delimited coordinate file into `GprCorRecord` (trace number, lat/lon, elevation, status).
- `GprDataParser` — parses `.data` binary file into `GprTrace` vectors. Supports 16-bit and 32-bit samples with configurable endianness (LE/BE). Reads entire file into buffer then deserializes trace-by-trace.
- `GprDatasetLoader` — facade that orchestrates the three parsers and produces a `GprDataset`.

**App (`app/`):** `gpr_demo` executable that loads a dataset by base path and prints a summary. Test data is auto-copied to the build directory via a post-build step.

## GPR File Formats

- `.head`: Text, `KEY: value` pairs with `#` comments. `DATA VERSION` = bits per sample (16 or 32).
- `.data`: Raw binary, `lastTrace * samples * (dataVersion/8)` bytes, stored trace-by-trace.
- `.cor`: Text, 10 whitespace-separated columns per line (trace#, date, time, northing, latType, easting, lonType, elevation, elevUnit, status).
- `.index` (not yet implemented): Text, marker positions by trace number.

See `数据文件-矩阵数据.md` for the full format specification.
