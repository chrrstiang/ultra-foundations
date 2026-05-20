package manager;

import java.time.LocalDate;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.concurrent.*;

/**
 * manager.Main class allows interaction with application through terminal
 * Simulates ultrasound scan.
 */
public class Main {

    public static void main(String[] args) {
        // inputs for Patient creation
        Scanner scanner = new Scanner(System.in);

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

        Frame[] frames = new Frame[0];
        ScanSession session = new ScanSession(p, frames, config);

        scanner.close();

        ArrayList<CompletableFuture<Frame>> completableFutures = new ArrayList<CompletableFuture<Frame>>();
        ExecutorService exec = Executors.newFixedThreadPool(5);

        for (int i = 0; i < 5; i++) {
            float[] arr = new RandomFloatArray(10).getData();

            CompletableFuture<Frame> future = CompletableFuture.supplyAsync(new Task(arr, i), exec);

            completableFutures.add(future);
        }

        // wait for the all threads to finish completing tasks
        CompletableFuture.allOf(completableFutures.toArray(CompletableFuture[]::new)).join();

        for (CompletableFuture<Frame> fut : completableFutures ) {
            try {
                Frame frame = fut.get();
                session.addFrame(frame);
            } catch (CancellationException | InterruptedException | ExecutionException c) {
                System.out.print("Error fetching frame from future; exiting");
                return;
            }
        }



    }
}
