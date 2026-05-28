package manager;

import java.time.Duration;
import java.time.LocalDateTime;
import java.util.Arrays;

/**
 * Contains information on the ultrasound scan session, including patient information,
 * image sequences, current state.
 */
public class ScanSession {
    final Patient patient;
    Frame[] imageSequence;
    State state;
    ScanConfiguration config;
    private LocalDateTime startTime;

    public ScanSession(Patient p, Frame[] f, ScanConfiguration c) {
        this.patient = p;
        this.imageSequence = f;
        this.state = State.CREATED;
        this.config = c;
    }

    /** Adds the frame to this ScanSession's image sequence. Checks the current state of the
     *  session to determine if frame can be added at this time or not.
     *
     * @param frame The frame being added to the imageSequence of this ScanSession
     */
    public void addFrame(Frame frame) {
        if (state != State.ACTIVE && state != State.STOPPED) {
            throw new IllegalStateException("Cannot add an image frame while session is not active.");
        }
        imageSequence = Arrays.copyOf(imageSequence, imageSequence.length + 1);
        imageSequence[imageSequence.length - 1] = frame;
    }

    // switches state of the ScanSession
    public void switchState() {
        switch (state) {
            case CREATED:
                state = State.STARTED;
                startTime = LocalDateTime.now();
                break;
            case STARTED:
                state = State.ACTIVE;
                break;
            case ACTIVE:
                state = State.STOPPED;
                break;
            case STOPPED:
                state = State.FINALIZED;
                break;
            case FINALIZED:
                break;
        }
    }

    /** Provides a summary of the session at its current state, including
     *  patient information, number of frames, duration, current state,
     *  and configuration settings
     */
    public void summary() {
        String duration = "N/A";
        if (startTime != null) {
            long seconds = Duration.between(startTime, LocalDateTime.now()).getSeconds();
            duration = seconds + "s";
        }

        System.out.println("Patient: " + patient.getFirstName() + " " + patient.getLastName()
                + " (ID: " + patient.getHospitalId() + ")");
        System.out.println("DOB: " + patient.getDateOfBirth());
        System.out.println("State: " + state);
        System.out.println("Frames: " + imageSequence.length);
        System.out.println("Duration: " + duration);
        System.out.println("Config — Depth: " + config.imageDepth + " cm | Frequency: "
                + config.frequency + " MHz | Gain: " + config.gain
                + " | Harmonic: " + config.harmonicMode);
    }
}
