# Java Scan Session Manager

An interactive command-line application that simulates an ultrasound scan session. It collects patient information and scan configuration, then concurrently generates image frames while letting the user control the session lifecycle through a simple terminal interface.

## Requirements

- Java 11+
- Maven 3.6+

## Build

Run from the repository root:

```bash
mvn compile
```

## Running the Application

```bash
mvn exec:java -Dexec.mainClass="manager.Main"
```

The application will prompt for:

1. **Patient information** — first name, last name, date of birth (YYYY-MM-DD)
2. **Scan configuration** — image depth (cm), frequency (MHz), gain (0.0–1.0), harmonic mode (y/n)

Once configured, a background thread pool begins generating frames. Use the terminal controls to manage the session:

| Key | Action |
|-----|--------|
| `s` | Print session summary (patient, config, frame count, state) |
| `q` | Advance session: ACTIVE → STOPPED → FINALIZED |

## Running Tests

```bash
mvn test
```

## Session Lifecycle

The scan session transitions through five states in order:

```
CREATED → STARTED → ACTIVE → STOPPED → FINALIZED
```

| State | Description |
|-------|-------------|
| `CREATED` | Session object initialized with patient and configuration |
| `STARTED` | Session started; background frame generation begins |
| `ACTIVE` | Frames are actively being acquired and added to the session |
| `STOPPED` | Frame generation halted; session data preserved |
| `FINALIZED` | Session closed; no further modifications allowed |

## Directory Layout

```
src/java/
├── main/
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
    └── manager/
        └── ScanSessionTest.java   # JUnit 5 tests for ScanSession lifecycle and behavior
```
