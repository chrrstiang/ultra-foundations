package manager;

import java.util.Date;

/**
 *  Patient class containing patient information for sessions of ultrasound.
 */
public class Patient {
    private final String firstName;
    private final String lastName;
    private final Date dateOfBirth;
    private final int hospitalId;

    Patient(String firstName, String lastName, Date dateOfBirth, int hospitalId) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.dateOfBirth = dateOfBirth;
        this.hospitalId = hospitalId;
    }

    public String getFirstName() {
        return this.firstName;
    }

    public String getLastName() {
        return this.lastName;
    }

    public Date getDateOfBirth() {
        return this.dateOfBirth;
    }

    public int getHospitalId() {
        return this.hospitalId;
    }

}
