package manager;

import java.time.LocalDateTime;

/**
 * Represents a frame of an image in the imaging sequence of the
 * ultrasound scan session.
 */
public class Frame {
    float[] pixelData;
    final LocalDateTime timestamp;
    int index;

    public Frame(float[] pixelData, int index) {
        this.pixelData = pixelData;
        this.timestamp = LocalDateTime.now();
        this.index = index;
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
