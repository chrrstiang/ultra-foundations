package manager;

/**
 * Contains information on the ultrasound scan session, including patient information,
 * image sequences, current state.
 */
public class ScanSession {
    private final Patient patient;
    private Frame[] imageSequence;
    State state;

    ScanSession(Patient p, Frame[] f, State s) {
        this.patient = p;
        this.imageSequence = f;
        this.state = s;
    }

    // validates and adds a frame to the image sequence
    public void addFrame() {

    }

    // begins the session
    public void begin() {

    }

    // provides a current summary of the session
    public void summary() {

    }
}
