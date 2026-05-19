import java.util.Random;

public class RandomFloatArray {
    private final float[] data;

    public RandomFloatArray(int size) {
        data = new float[size];
        Random rand = new Random();
        for (int i = 0; i < size; i++) {
            data[i] = rand.nextFloat();
        }
    }

    public float[] getData() {
        return data;
    }
}