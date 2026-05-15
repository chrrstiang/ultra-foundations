package manager;

import java.time.LocalDateTime;

/**
 * Represents a frame of an image in the imaging sequence of the
 * ultrasound scan session.
 */
public class Frame {
    private float[] pixelData;
    private final LocalDateTime timestamp;
    int index;
    ScanConfiguration settings;

    Frame(float[] pixelData, LocalDateTime timestamp, int index, ScanConfiguration settings) {
        this.pixelData = pixelData;
        this.timestamp = timestamp;
        this.index = index;
        this.settings  = settings;
    }

    // average intensity of pixel data aka float averages
    public float averageIntensity() {
        return 1;
    }

    // determines if an image frame is corrupted
    boolean isCorrupted() {
        return false;
    }

}
