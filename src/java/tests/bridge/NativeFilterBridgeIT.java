package bridge;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.*;

/**
 * Integration tests for NativeFilterBridge. Requires the imageFilterLib native
 * library to be built first (make build-cpp). Run via: make test-java-bridge
 */
public class NativeFilterBridgeIT {

    private NativeFilterBridge bridge;

    @BeforeEach
    void setUp() {
        bridge = new NativeFilterBridge();
        // reset C++ pipeline state between tests — empty filter array = passthrough
        bridge.configureFilters(new int[]{});
    }

    // --- processImage with no pipeline configured ---

    @Test
    void processImage_noPipelineConfigured_returnsInputUnchanged() {
        // configureFilters was never called — g_pipeline is null on first JVM load
        // (setUp calls configureFilters([]) which sets an empty pipeline, same effect)
        float[] input = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        float[] output = bridge.processImage(input);
        assertArrayEquals(input, output);
    }

    // --- empty filter array ---

    @Test
    void processImage_emptyFilterArray_returnsInputUnchanged() {
        bridge.configureFilters(new int[]{});
        float[] input = {0.1f, 0.5f, 0.9f, 0.3f, 0.7f};
        float[] output = bridge.processImage(input);
        assertArrayEquals(input, output);
    }

    // --- length invariant ---

    @Test
    void processImage_returnsSameLengthArray() {
        bridge.configureFilters(new int[]{NativeFilterBridge.FILTER_GAUSSIAN});
        float[] input = {0.5f, 0.3f, 0.8f, 0.1f, 0.9f, 0.4f, 0.6f, 0.2f, 0.7f, 0.0f};
        float[] output = bridge.processImage(input);
        assertEquals(input.length, output.length);
    }

    // --- intensity normalization: output in [0, 255] ---

    @Test
    void processImage_withIntensityNorm_outputInRange0to255() {
        bridge.configureFilters(new int[]{NativeFilterBridge.FILTER_NORMALIZE});
        float[] input = {10.0f, 50.0f, 20.0f, 80.0f, 5.0f, 100.0f, 30.0f, 60.0f, 15.0f, 90.0f};
        float[] output = bridge.processImage(input);
        for (float v : output) {
            assertTrue(v >= 0.0f && v <= 255.0f,
                    "Expected value in [0, 255] but got: " + v);
        }
    }

    // --- Gaussian blur changes output for alternating signal ---

    @Test
    void processImage_withGaussian_outputDiffersFromInput() {
        bridge.configureFilters(new int[]{NativeFilterBridge.FILTER_GAUSSIAN});
        // alternating high/low values — Gaussian will smooth them
        float[] input = {100.0f, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f};
        float[] output = bridge.processImage(input);
        assertFalse(Arrays.equals(input, output),
                "Gaussian blur should change alternating input values");
    }

    // --- reconfiguring filters replaces the previous pipeline ---

    @Test
    void configureFilters_canBeReconfigured() {
        float[] input = {100.0f, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f, 100.0f, 0.0f};

        bridge.configureFilters(new int[]{NativeFilterBridge.FILTER_GAUSSIAN});
        float[] withGaussian = bridge.processImage(input.clone());

        bridge.configureFilters(new int[]{});
        float[] withNoFilters = bridge.processImage(input.clone());

        // after clearing filters, output should equal input
        assertArrayEquals(input, withNoFilters,
                "Empty pipeline should pass input through unchanged");
        // the two outputs should differ
        assertFalse(Arrays.equals(withGaussian, withNoFilters),
                "Gaussian output should differ from passthrough output");
    }

    // --- all three filters together ---

    @Test
    void processImage_withAllFilters_returnsValidResult() {
        bridge.configureFilters(new int[]{
                NativeFilterBridge.FILTER_GAUSSIAN,
                NativeFilterBridge.FILTER_SOBEL,
                NativeFilterBridge.FILTER_NORMALIZE
        });
        float[] input = {50.0f, 100.0f, 150.0f, 200.0f, 50.0f, 100.0f, 150.0f, 200.0f, 50.0f, 100.0f};
        float[] output = bridge.processImage(input);
        assertNotNull(output);
        assertEquals(input.length, output.length);
        for (float v : output) {
            assertFalse(Float.isNaN(v), "Output must not contain NaN");
            assertFalse(Float.isInfinite(v), "Output must not contain Infinity");
        }
    }

    // --- unknown filter IDs are silently ignored ---

    @Test
    void configureFilters_unknownIds_noExceptionAndPassthrough() {
        assertDoesNotThrow(() -> bridge.configureFilters(new int[]{99, 100, -1}));
        float[] input = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        float[] output = bridge.processImage(input);
        // no valid filters configured — pipeline is empty → passthrough
        assertArrayEquals(input, output);
    }

    // --- integration: consistent results across multiple calls ---

    @Test
    void processImage_consistentResultsAcrossMultipleCalls() {
        bridge.configureFilters(new int[]{NativeFilterBridge.FILTER_NORMALIZE});
        float[] input = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
        float[] output1 = bridge.processImage(input.clone());
        float[] output2 = bridge.processImage(input.clone());
        assertArrayEquals(output1, output2,
                "Same input with same pipeline should produce identical results");
    }

    // --- integration: intensity normalization min becomes 0, max becomes 255 ---

    @Test
    void processImage_withIntensityNorm_minIsZeroMaxIs255() {
        bridge.configureFilters(new int[]{NativeFilterBridge.FILTER_NORMALIZE});
        float[] input = {10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f};
        float[] output = bridge.processImage(input);
        float min = Float.MAX_VALUE;
        float max = Float.MIN_VALUE;
        for (float v : output) {
            if (v < min) min = v;
            if (v > max) max = v;
        }
        assertEquals(0.0f, min, 0.001f, "Normalized min should be 0");
        assertEquals(255.0f, max, 0.001f, "Normalized max should be 255");
    }
}
