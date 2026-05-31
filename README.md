# ultra-foundations

A mono-repo containing two independent foundational projects built around medical imaging concepts: a Java ultrasound scan session manager and a C++ image processing pipeline.

## Sub-projects

| Sub-project | Language | Description |
|-------------|----------|-------------|
| [Scan Session Manager](src/java/README.md) | Java 11 | Interactive CLI that simulates acquiring ultrasound frames, managing patient records, and driving a scan session through its full lifecycle |
| [Image Processing Pipeline](src/cpp/README.md) | C++17 | Command-line tool that applies a configurable filter pipeline (Gaussian blur, Sobel edge detection, intensity normalization) to PGM grayscale images, with optional multi-threaded parallelization |

## Repository Layout

```
ultra-foundations/
├── src/
│   ├── cpp/          # C++ image processing pipeline
│   │   ├── main/     # Source code
│   │   └── tests/    # Google Test unit tests
│   └── java/         # Java scan session manager
│       ├── main/     # Source code
│       └── tests/    # JUnit 5 unit tests
├── CMakeLists.txt    # C++ build configuration
├── pom.xml           # Java build configuration (Maven)
└── README.md
```

## Quick Start

**C++ pipeline** — see [src/cpp/README.md](src/cpp/README.md) for full details:
```bash
cmake -S . -B build
cmake --build build
./build/ultra-foundations <input.pgm> <output.pgm> [--parallel]
```

**Java scan manager** — see [src/java/README.md](src/java/README.md) for full details:
```bash
mvn compile
mvn exec:java -Dexec.mainClass="manager.Main"
```
