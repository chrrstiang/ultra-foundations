package manager;

import bridge.NativeFilterBridge;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.*;

/**
 * manager.Main class allows interaction with application through terminal
 * Simulates ultrasound scan.
 */
public class Main {

    public static void main(String[] args) throws InterruptedException {
        Scanner scanner = new Scanner(System.in);

        // inputs for Patient creation
        System.out.print("What's your first name? ");
        String firstName = scanner.nextLine();

        System.out.print("What's your last name? ");
        String lastName = scanner.nextLine();

        System.out.print("Date of Birth (YYYY-MM-DD): ");
        LocalDate dateOfBirth = LocalDate.parse(scanner.nextLine());

        Patient p = new Patient(firstName, lastName, dateOfBirth);

        // inputs for ScanConfiguration creation
        System.out.print("Image depth (cm): ");
        float imageDepth = Float.parseFloat(scanner.nextLine());

        System.out.print("Frequency (MHz): ");
        float frequency = Float.parseFloat(scanner.nextLine());

        System.out.print("Gain (0.0 - 1.0): ");
        float gain = Float.parseFloat(scanner.nextLine());

        System.out.print("Harmonic mode? (y / n): ");
        boolean harmonicMode = scanner.nextLine().equals("y");

        ScanConfiguration config = ScanConfiguration.ConfigurationBuilder.newInstance()
                .setImageDepth(imageDepth)
                .setFrequency(frequency)
                .setGain(gain)
                .setHarmonicMode(harmonicMode)
                .build();

        ScanSession session = new ScanSession(p, new Frame[0], config);

        // filter selection — configure C++ pipeline before scan begins
        NativeFilterBridge bridge = new NativeFilterBridge();
        ArrayList<Integer> selectedFilters = new ArrayList<>();

        System.out.print("Apply Gaussian Blur? (y/n): ");
        if (scanner.nextLine().trim().equals("y")) selectedFilters.add(NativeFilterBridge.FILTER_GAUSSIAN);

        System.out.print("Apply Sobel Edge Detection? (y/n): ");
        if (scanner.nextLine().trim().equals("y")) selectedFilters.add(NativeFilterBridge.FILTER_SOBEL);

        System.out.print("Apply Intensity Normalization? (y/n): ");
        if (scanner.nextLine().trim().equals("y")) selectedFilters.add(NativeFilterBridge.FILTER_NORMALIZE);

        int[] filterIds = selectedFilters.stream().mapToInt(Integer::intValue).toArray();
        bridge.configureFilters(filterIds);

        // wait for Enter to start
        System.out.println("Press Enter to start the scan...");
        scanner.nextLine();

        session.switchState(); // CREATED -> STARTED
        session.switchState(); // STARTED -> ACTIVE
        System.out.println("Scan active. Press 's' for summary, 'q' to stop.");

        // frame generation thread — preserves existing CompletableFuture batch pattern
        Thread frameThread = new Thread(() -> {
            int index = 0;
            ExecutorService exec = Executors.newFixedThreadPool(5);
            while (session.state == State.ACTIVE) {
                ArrayList<CompletableFuture<Frame>> futures = new ArrayList<>();
                for (int i = 0; i < 5; i++) {
                    float[] arr = new RandomFloatArray(10).getData();
                    futures.add(CompletableFuture.supplyAsync(new Task(arr, index + i), exec));
                }
                index += 5;
                CompletableFuture.allOf(futures.toArray(CompletableFuture[]::new)).join();
                for (CompletableFuture<Frame> fut : futures) {
                    try {
                        Frame raw = fut.get();
                        float[] filtered = bridge.processImage(raw.pixelData);
                        session.addFrame(new Frame(filtered, raw.index));
                    } catch (IllegalStateException | InterruptedException |
                             ExecutionException e) {
                        exec.shutdown();
                        return;
                    }
                }
            }
            exec.shutdown();
        });
        frameThread.setDaemon(true);
        frameThread.start();

        // input loop — controls session lifecycle
        while (true) {
            String input = scanner.nextLine().trim();
            if (input.equals("s") && session.state == State.ACTIVE) {
                session.summary();
            } else if (input.equals("q") && session.state == State.ACTIVE) {
                session.switchState(); // ACTIVE -> STOPPED
                System.out.println("Scan stopped. Press 'q' to finalize.");
            } else if (input.equals("q") && session.state == State.STOPPED) {
                session.switchState(); // STOPPED -> FINALIZED
                frameThread.join();
                session.summary();
                break;
            }
        }

        scanner.close();
    }
}
