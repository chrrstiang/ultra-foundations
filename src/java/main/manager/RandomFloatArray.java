package manager;

import java.util.Arrays;
import java.util.concurrent.ThreadLocalRandom;

public class RandomFloatArray {
    private final float[] data;

    public RandomFloatArray(int size) {
        data = new float[size];
        for (int i = 0; i < size; i++) {
            data[i] = ThreadLocalRandom.current().nextFloat();
        }
    }

    public float[] getData() {
        return Arrays.copyOf(data, data.length);
    }
}