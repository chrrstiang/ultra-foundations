# Java Scan Session Manager

An interactive command-line application that simulates an ultrasound scan session. It collects patient information, scan configuration, and filter selection, then concurrently generates image frames — each filtered through the C++ pipeline via JNI — while letting the user control the session lifecycle through a simple terminal interface.

## Requirements

- Java 11+
- Maven 3.6+ (for tests only)

## Build

Run from the repository root:

```bash
make build-java    # compiles all Java sources to out/
```

## Running the Application

```bash
make run-scan
```

The application prompts for input in four phases:

1. **Patient information** — first name, last name, date of birth (YYYY-MM-DD)
2. **Scan configuration** — image depth (cm), frequency (MHz), gain (0.0–1.0), harmonic mode (y/n)
3. **Filter selection** — choose which filters the C++ pipeline applies to every frame:
   - `Apply Gaussian Blur? (y/n)`
   - `Apply Sobel Edge Detection? (y/n)`
   - `Apply Intensity Normalization? (y/n)`
4. **Scan controls** — press Enter to start, then use terminal commands:

| Key | Action |
|-----|--------|
| `s` | Print session summary (patient, config, frame count, state) |
| `q` | Advance session: ACTIVE → STOPPED → FINALIZED |

## JNI Bridge

Each frame generated during the scan has its pixel data sent through `NativeFilterBridge.processImage()` before being added to the session. The bridge calls into the C++ `imageFilterLib` shared library, where the configured filter pipeline is applied. Filter selection at scan start calls `NativeFilterBridge.configureFilters(int[])` once — the same pipeline is reused for every subsequent frame.

Frames are generated in concurrent batches of 5 via a fixed thread pool. Each frame's raw pixel data is filtered before it enters `ScanSession.addFrame()`.

## Session Lifecycle

The scan session transitions through five states in order:

```
CREATED → STARTED → ACTIVE → STOPPED → FINALIZED
```

| State | Description |
|-------|-------------|
| `CREATED` | Session object initialized with patient and configuration |
| `STARTED` | Session started; background frame generation begins |
| `ACTIVE` | Frames are actively being acquired, filtered, and added to the session |
| `STOPPED` | Frame generation halted; session data preserved |
| `FINALIZED` | Session closed; no further modifications allowed |

## Running Tests

```bash
make test-java          # JUnit 5 unit tests (ScanSession lifecycle)
make test-java-bridge   # JNI integration tests (requires make build-cpp first)
```

The JNI integration tests in `NativeFilterBridgeIT` load the real native library and exercise:
- Passthrough with no pipeline / empty filter array
- Array length invariant across all filter configurations
- Intensity normalization range [0, 255]
- Gaussian blur output differs from input
- Pipeline reconfiguration at runtime
- All three filters applied together
- Unknown filter IDs silently ignored
- Consistent results across repeated calls
- Min=0 / Max=255 normalization contract

## Directory Layout

```
src/java/
├── main/
│   ├── bridge/
│   │   └── NativeFilterBridge.java  # JNI bridge — configureFilters, processImage
│   └── manager/
│       ├── Main.java              # Entry point — collects input, manages session loop
│       ├── ScanSession.java       # Stateful container: patient, frames, config, and state
│       ├── Patient.java           # Immutable patient record (name, DOB, hospital ID)
│       ├── ScanConfiguration.java # Builder-pattern scan settings (depth, frequency, gain, harmonic)
│       ├── Frame.java             # Single image frame: pixel data (float[]), timestamp, index
│       ├── State.java             # Enum: CREATED, STARTED, ACTIVE, STOPPED, FINALIZED
│       ├── Task.java              # Callable/Supplier that produces a Frame
│       └── RandomFloatArray.java  # Generates random float arrays for simulated pixel data
└── tests/
    ├── bridge/
    │   └── NativeFilterBridgeIT.java  # JUnit 5 integration tests for JNI bridge (10 tests)
    └── manager/
        └── ScanSessionTest.java       # JUnit 5 tests for ScanSession lifecycle and behavior
```
