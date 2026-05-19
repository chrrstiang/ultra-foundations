package manager;

/**
 * Contains information on the ultrasound scan session, including patient information,
 * image sequences, current state.
 */
public class ScanSession {
    private final Patient patient;
    private Frame[] imageSequence;
    State state;
    ScanConfiguration config;

    public ScanSession(Patient p, Frame[] f, ScanConfiguration c) {
        this.patient = p;
        this.imageSequence = f;
        this.state = State.CREATED;
        this.config = c;
    }

    // validates and adds a frame to the image sequence depending on the current state
    public void addFrame(Frame frame) {

    }

    // switches the state of the session
    public void switchState() {

    }

    // provides a current summary of the session
    public void summary() {

    }
}
