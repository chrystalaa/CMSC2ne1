#ifndef BOOK_APPOINTMENT_H
#define BOOK_APPOINTMENT_H

#include <stdio.h>
#include <string.h>

#include "HelperFunctions.h"
#include "DatePicker.h"
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
    if (!pickDate(na.date)) return;


    /* Type */
    const char *types[] = {"Pregnant", "Senior", "PWD", "Regular"};
    int typeChoice;
    while (1) {
        printf("\nAppointment type:\n");
        for (int i = 0; i < 4; i++)
            printf("  %d. %s\n", i + 1, types[i]);
        printf("  0. Go back\n");
        printf("Choice: ");

        if (scanf("%d", &typeChoice) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');

        if (typeChoice == 0) { printf("Going back.\n"); return; }
        if (typeChoice >= 1 && typeChoice <= 4) {
            strcpy(na.type, types[typeChoice - 1]);
            break;
        }
        printf("Invalid choice.\n");
    }
    /* Show only doctors free on the chosen date */
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