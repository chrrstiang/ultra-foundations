# C++ Image Processing Pipeline

A command-line image processing tool that reads a PGM grayscale image, applies a three-stage filter pipeline, and writes the result back to disk. Supports both sequential and multi-threaded parallel execution.

## Requirements

- CMake 3.14+
- C++17 compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- Internet access on first build (CMake fetches GoogleTest automatically)

## Build

Run from the repository root:

```bash
cmake -S . -B build
cmake --build build
```

The executable is produced at `build/ultra-foundations`.

## Usage

```bash
./build/ultra-foundations <input.pgm> <output.pgm> [--parallel]
```

| Argument | Description |
|----------|-------------|
| `<input.pgm>` | Path to a P5 (binary) PGM grayscale image |
| `<output.pgm>` | Path where the processed image will be written |
| `--parallel` | Optional flag to enable multi-threaded strip-based processing |

**Example:**
```bash
./build/ultra-foundations baboon.ascii.pgm output.pgm --parallel
```

## Filter Pipeline

Filters are applied in this fixed order:

| Stage | Filter | Description |
|-------|--------|-------------|
| 1 | Gaussian Blur | Smooths the image using a 3x3 kernel (weights: 1 2 1 / 2 4 2 / 1 2 1, sum 16) |
| 2 | Sobel Edge Detection | Computes gradient magnitude using Kx/Ky kernels; highlights edges |
| 3 | Intensity Normalization | Stretches pixel values to the full [0, 255] range |

### Parallel Execution

With `--parallel`, the image is divided into horizontal strips (one per hardware thread). Each strip is processed concurrently via `std::async`. Ghost rows (one row of overlap on each edge) are included during filtering to prevent border artifacts, then trimmed before recombining. A serial pre-pass calls `prepare()` on each filter with the full image before slicing, ensuring global state (e.g. intensity normalization min/max) is computed correctly across all strips.

## Parallelization Performance

Benchmarked against the same three-filter pipeline (Gaussian blur → Sobel edge detection → intensity normalization) on the same input image, run 10 times each:

| Mode | Average time |
|------|-------------|
| Sequential | ~baseline |
| Parallel (`--parallel`) | ~2.5x faster |

Parallel execution produced an average **250% performance increase** over sequential. Gains scale with the number of hardware threads available — the parallelizer uses `std::thread::hardware_concurrency()` to determine strip count, so results will vary by machine.

Output is pixel-identical between modes. The serial pre-pass for `IntensityNormalization::prepare()` ensures global min/max is computed from the full image before strips are distributed, so parallelization does not affect result correctness.

## Running Tests

```bash
cmake --build build
cd build && ctest --output-on-failure
```

76 tests across 8 test suites covering success, failure, and edge cases.

## Directory Layout

```
src/cpp/
├── main/
│   ├── main.cpp              # Entry point — argument parsing, pipeline construction
│   ├── image/
│   │   ├── image.h           # Image class declaration
│   │   └── image.cpp         # 2D float matrix; supports slice() and combine() for parallelization
│   ├── filters/
│   │   ├── filter.h          # Abstract Filter base class (apply, prepare)
│   │   ├── filter.cpp
│   │   ├── gaussian.h/cpp    # GaussianBlur filter
│   │   ├── sobel_edge_detection.h/cpp   # SobelEdgeDetection filter
│   │   └── intensity_normalization.h/cpp # IntensityNormalization filter
│   ├── pipeline/
│   │   ├── filter_pipeline.h/cpp  # Pipeline — composes filters, drives execution
│   ├── parallelizer/
│   │   ├── parallelizer.h/cpp     # Strips image into rows, runs filters via std::async
│   ├── parser/
│   │   ├── pgm_parser.h/cpp       # Reads P5 PGM files into Image objects
│   ├── writer/
│   │   ├── pgm_writer.h/cpp       # Writes Image objects to P5 PGM files
│   └── buffer/
│       └── ring_buffer.h          # Generic circular buffer template (RingBuffer<T>)
└── tests/
    ├── CMakeLists.txt
    ├── image_tests.cpp
    ├── gaussian_tests.cpp
    ├── sobel_edge_detection_tests.cpp
    ├── intensity_normalization_tests.cpp
    ├── parallelizer_tests.cpp
    ├── pgm_parser_tests.cpp
    ├── pgm_writer_tests.cpp
    └── ring_buffer_tests.cpp
```
