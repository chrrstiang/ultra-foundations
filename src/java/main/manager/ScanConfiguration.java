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

    public ScanConfiguration(ConfigurationBuilder builder) {
        this.imageDepth = builder.imageDepth;
        this.frequency = builder.frequency;
        this.gain = builder.gain;
        this.harmonicMode = builder.harmonicMode;
    }

    public static class ConfigurationBuilder {
        private float imageDepth;
        private float frequency;
        private float gain;
        private boolean harmonicMode;

        public static ConfigurationBuilder newInstance() {
            return new ConfigurationBuilder();
        }

        public ConfigurationBuilder setImageDepth(float d) {
            this.imageDepth = d;
            return this;
        }

        public ConfigurationBuilder setFrequency(float f) {
            this.frequency = f;
            return this;
        }

        public ConfigurationBuilder setGain(float g) {
            this.gain = g;
            return this;
        }

        public ConfigurationBuilder setHarmonicMode(boolean harmonicMode) {
            this.harmonicMode = harmonicMode;
            return this;
        }

        public ScanConfiguration build() {
            return new ScanConfiguration(this);
        }
    }
}
