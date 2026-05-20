package manager;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.time.LocalDate;

import static org.junit.jupiter.api.Assertions.*;

public class ScanSessionTest {

    private Patient patient;
    private ScanConfiguration config;
    private ScanSession session;

    @BeforeEach
    void setUp() {
        patient = new Patient("John", "Doe", LocalDate.of(1990, 1, 1));
        config = ScanConfiguration.ConfigurationBuilder.newInstance()
                .setImageDepth(10f)
                .setFrequency(3.5f)
                .setGain(0.5f)
                .setHarmonicMode(false)
                .build();
        session = new ScanSession(patient, new Frame[]{}, config);
    }

    // --- Constructor ---

    @Test
    void constructor_setsInitialStateToCREATED() {
        assertEquals(State.CREATED, session.state);
    }

    @Test
    void constructor_storesPatient() {
        assertSame(patient, session.patient);
    }

    @Test
    void constructor_storesConfig() {
        assertSame(config, session.config);
    }

    @Test
    void constructor_storesImageSequence() {
        assertEquals(0, session.imageSequence.length);
    }

    // --- addFrame ---

    @Test
    void addFrame_appendsFrameToImageSequence() {
        Frame frame = new Frame(new float[]{0.1f, 0.5f, 0.9f}, 0);
        session.addFrame(frame);
        assertEquals(1, session.imageSequence.length);
        assertSame(frame, session.imageSequence[0]);
    }

    @Test
    void addFrame_doesNotAddFrameWhenFinalized() {
        // advance to FINALIZED
        session.switchState(); // CREATED -> STARTED
        session.switchState(); // STARTED -> ACTIVE
        session.switchState(); // ACTIVE  -> STOPPED
        session.switchState(); // STOPPED -> FINALIZED

        Frame frame = new Frame(new float[]{0.2f, 0.4f}, 1);
        session.addFrame(frame);
        assertEquals(0, session.imageSequence.length);
    }

    // --- switchState ---

    @Test
    void switchState_advancesFromCREATEDtoSTARTED() {
        session.switchState();
        assertEquals(State.STARTED, session.state);
    }

    @Test
    void switchState_advancesFromSTARTEDtoACTIVE() {
        session.switchState();
        session.switchState();
        assertEquals(State.ACTIVE, session.state);
    }

    @Test
    void switchState_advancesFromACTIVEtoSTOPPED() {
        session.switchState();
        session.switchState();
        session.switchState();
        assertEquals(State.STOPPED, session.state);
    }

    @Test
    void switchState_advancesFromSTOPPEDtoFINALIZED() {
        session.switchState();
        session.switchState();
        session.switchState();
        session.switchState();
        assertEquals(State.FINALIZED, session.state);
    }

    // --- summary ---

    @Test
    void summary_doesNotThrow() {
        assertDoesNotThrow(() -> session.summary());
    }
}
