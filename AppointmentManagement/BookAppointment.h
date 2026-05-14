#ifndef BOOK_APPOINTMENT_H
#define BOOK_APPOINTMENT_H

#include <stdio.h>
#include <string.h>
#include "AppointmentMgmt.h"


/* ===========================================================
   1. BOOK APPOINTMENT
   =========================================================== */
static inline void bookAppointment(Appointment *appts, int *total,
                                   Doctor doctors[], int doctorCount) {
    if (*total >= MAX_APPOINTMENTS) {
        printf("Cannot book: maximum capacity (%d) reached.\n", MAX_APPOINTMENTS);
        return;
    }

    Appointment na;
    memset(&na, 0, sizeof(na));

    while (getchar() != '\n');   /* flush */

    printf("\n--- Book Appointment (enter 0 at any prompt to go back) ---\n");

    /* Patient name */
    printf("Patient name: ");
    fgets(na.patientName, sizeof(na.patientName), stdin);
    na.patientName[strcspn(na.patientName, "\n")] = '\0';
    if (strcmp(na.patientName, "0") == 0 || na.patientName[0] == '\0') {
        printf("Going back.\n"); return;
    }

    /* Date */
    printf("Appointment date (YYYY-MM-DD): ");
    fgets(na.date, sizeof(na.date), stdin);
    na.date[strcspn(na.date, "\n")] = '\0';
    if (strcmp(na.date, "0") == 0 || na.date[0] == '\0') {
        printf("Going back.\n"); return;
    }

    /* Show only doctors free on the chosen date (Unit 5: Arrays) */
    int available[MAX_DOCTORS];
    int availCount = 0;

    printf("\nAvailable doctors on %s:\n", na.date);
    for (int i = 0; i < doctorCount; i++) {
        if (canBookDoctor(appts, *total, doctors[i].name, na.date)) {
            printf("  %d. Dr. %-22s (%s)\n",
                   availCount + 1, doctors[i].name, doctors[i].specialty);
            available[availCount++] = i;
        }
    }

    if (availCount == 0) {
        printf("No doctors available on %s.\n", na.date); return;
    }
    printf("  0. Go back\n");
    

    int choice;
    printf("Select doctor (0-%d): ", availCount);
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n') {}
        return;
    }
    if (choice == 0) {
        printf("Going back.\n"); while (getchar() != '\n'); return;
    }
    if (choice < 1 || choice > availCount) {
        printf("Invalid choice.\n"); while (getchar() != '\n'); return;
    }

    strcpy(na.doctorName, doctors[available[choice - 1]].name);
    generateAppointmentID(na.appointmentID, na.doctorName, doctors, doctorCount);
    strcpy(na.status, "Scheduled");

    appts[*total] = na;
    (*total)++;
    saveAppointments(appts, *total);

    printf("\nAppointment booked successfully!\n");
    printAppointment(&appts[*total - 1]);
}

#endif /* BOOK_APPOINTMENT_H */