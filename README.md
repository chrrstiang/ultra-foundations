# ultra-foundations

A mono-repo containing two foundational projects built around medical imaging concepts: a Java ultrasound scan session manager and a C++ image processing pipeline. The two sub-projects are integrated via a JNI bridge — every frame acquired during a scan is processed through the C++ filter pipeline before being stored in the session.

## Sub-projects

| Sub-project | Language | Description |
|-------------|----------|-------------|
| [Scan Session Manager](src/java/README.md) | Java 11 | Interactive CLI that simulates acquiring ultrasound frames, applies C++ image filters via JNI, and drives a scan session through its full lifecycle |
| [Image Processing Pipeline](src/cpp/README.md) | C++17 | Command-line tool and shared library that applies a configurable filter pipeline (Gaussian blur, Sobel edge detection, intensity normalization) to grayscale images, with optional multi-threaded parallelization |

## Repository Layout

```
ultra-foundations/
├── src/
│   ├── cpp/          # C++ image processing pipeline
│   │   ├── main/     # Source code + JNI bridge
│   │   └── tests/    # Google Test unit tests
│   └── java/         # Java scan session manager
│       ├── main/     # Source code + JNI bridge class
│       └── tests/    # JUnit 5 unit + integration tests
├── CMakeLists.txt    # C++ build configuration
├── pom.xml           # Java build configuration (Maven)
├── Makefile          # Primary build and run entrypoint
└── README.md
```

## Quick Start

**C++ pipeline** — see [src/cpp/README.md](src/cpp/README.md) for full details:
```bash
make build-cpp
./build/ultra-foundations <input.pgm> <output.pgm> [--parallel]
```

**Java scan manager** — see [src/java/README.md](src/java/README.md) for full details:
```bash
make run-scan
```

## Common Makefile Targets

| Target | Description |
|--------|-------------|
| `make build-cpp` | Build C++ executable and `imageFilterLib` shared library |
| `make build-java` | Compile all Java sources to `out/` |
| `make run-scan` | Build everything and launch the interactive scan session |
| `make run-pipeline` | Run C++ pipeline on `baboon.ascii.pgm` |
| `make run-bridge` | Test the JNI bridge standalone |
| `make test-cpp` | Run 76 C++ unit tests via ctest |
| `make test-java` | Run Java unit tests via Maven |
| `make test-java-bridge` | Run JNI integration tests via Maven Failsafe |
| `make lint-cpp` | Run clang-tidy on all C++ sources |
| `make lint-java` | Run SpotBugs on compiled Java bytecode |
| `make lint` | Run both linters |
