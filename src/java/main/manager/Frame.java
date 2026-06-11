package manager;

import java.time.LocalDateTime;
import java.util.Arrays;

/**
 * Represents a frame of an image in the imaging sequence of the
 * ultrasound scan session.
 */
public class Frame {
    float[] pixelData;
    final LocalDateTime timestamp;
    int index;

    public Frame(float[] pixelData, int index) {
        this.pixelData = Arrays.copyOf(pixelData, pixelData.length);
        this.timestamp = LocalDateTime.now();
        this.index = index;
    }

    // average intensity of pixel data aka float averages
    public float averageIntensity() {
        float sum = 0;
        for (float f : pixelData) {
            sum += f;
        }
        return sum / pixelData.length;
    }

}
