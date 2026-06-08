package bridge;

import java.util.Arrays;

/**
 * Bridge class that contains the processImage function.
 */
public class NativeFilterBridge {
    static {
        System.loadLibrary("imageFilterLib");
    }

    /**
     * Provides a byte array (image pixel data) to the C++ image filter pipeline
     * 
     * @param byteArray the pixel data of the frame
     * @return an updated byteArray with applied filters
     */
    public native float[] processImage(float[] byteArray);

    public static void main(String[] args) {
        NativeFilterBridge fil = new NativeFilterBridge();

        float[] byteArray = new float[3];
        byteArray[0] = 1;
        byteArray[1] = 2;
        byteArray[2] = 3;

        System.out.println("byteArray is " + Arrays.toString(byteArray));

        float[] newFloats = fil.processImage(byteArray);

        System.out.println("New array is " + Arrays.toString(newFloats));
    }
}