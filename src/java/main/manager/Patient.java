package manager;

import java.time.LocalDate;

/**
 *  Patient class containing patient information for sessions of ultrasound.
 */
public class Patient {
    private final String firstName;
    private final String lastName;
    private final LocalDate dateOfBirth;
    private final int hospitalId;

    public Patient(String firstName, String lastName, LocalDate dateOfBirth) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.dateOfBirth = dateOfBirth;
        this.hospitalId = 10;
    }

    public String getFirstName() {
        return this.firstName;
    }

    public String getLastName() {
        return this.lastName;
    }

    public LocalDate getDateOfBirth() {
        return this.dateOfBirth;
    }

    public int getHospitalId() {
        return this.hospitalId;
    }

}
