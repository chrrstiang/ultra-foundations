package manager;

/**
 * ScanConfiguration class contains information about the settings
 * that the ultrasound scan uses for imaging.
 */
public class ScanConfiguration {
    private float imageDepth;
    private float frequency;
    private float gain;
    private boolean harmonicMode;

    ScanConfiguration(ConfigurationBuilder builder) {
        this.imageDepth = builder.imageDepth;
        this.frequency = builder.frequency;
        this.gain = builder.gain;
        this.harmonicMode = builder.harmonicMode;
    }

    static class ConfigurationBuilder {
        private float imageDepth;
        private float frequency;
        private float gain;
        private boolean harmonicMode;

        public static ConfigurationBuilder newInstance() {
            return new ConfigurationBuilder();
        }

        private ConfigurationBuilder setImageDepth(float d) {
            this.imageDepth = d;
            return this;
        }

        private ConfigurationBuilder setFrequency(float f) {
            this.frequency = f;
            return this;
        }

        private ConfigurationBuilder setGain(float g) {
            this.gain = g;
            return this;
        }

        private ConfigurationBuilder setHarmonicMode(boolean harmonicMode) {
            this.harmonicMode = harmonicMode;
            return this;
        }

        public ScanConfiguration build() {
            return new ScanConfiguration(this);
        }
    }
}
