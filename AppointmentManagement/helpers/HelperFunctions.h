#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ===========================================================
   HELPER FUNCTIONS
   =========================================================== */

/*
 * Returns 1 if the given doctor has NO Scheduled appointment on 'date'.
 * Used to prevent double-booking the same doctor on the same day.
 */
static inline int canBookDoctor(Appointment *appts, int total,
                                const char *doctorName, const char *date) {
    int appt_count = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(appts[i].doctorName, doctorName) == 0 &&
            strcmp(appts[i].date,       date)       == 0 &&
            strcmp(appts[i].status,     "Scheduled") == 0)
            appt_count++;
    }

    return (appt_count < 5);
}

/*
 * Generates an appointment ID in the form "D<docIdx>-<counter>".
 * Counter is persisted in appt_counter.txt so IDs are unique across runs.
 */
static inline void generateAppointmentID(char *id,
                                         const char *doctorName,
                                         Doctor doctors[], int doctorCount) {
    /* Find the doctor's 1-based index */
    int docIdx = 0;
    for (int i = 0; i < doctorCount; i++) {
        if (strcmp(doctors[i].name, doctorName) == 0) { docIdx = i + 1; break; }
    }

    /* Read persistent counter */
    int counter = 1;
    FILE *cf = fopen("records/appt_counter.txt", "r");
    if (cf) { fscanf(cf, "%d", &counter); fclose(cf); }

    snprintf(id, 9, "D%d-%04d", docIdx, counter);

    /* Increment and save counter for next booking */
    cf = fopen("appt_counter.txt", "w");
    if (cf) { fprintf(cf, "%d", counter + 1); fclose(cf); }
}

/* Pretty-print one appointment */
static inline void printAppointment(Appointment *a) {
    printf("  ID     : %s\n", a->appointmentID);
    printf("  Patient: %s\n", a->patientName);
    printf("  Doctor : %s\n", a->doctorName);
    printf("  Date   : %s\n", a->date);
    printf("  Type   : %s\n", a->type);
    printf("  Status : %s\n", a->status);
}

#endif /* HELPERFUNCTIONS_H */