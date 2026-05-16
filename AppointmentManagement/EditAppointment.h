#ifndef EDIT_APPOINTMENT_H
#define EDIT_APPOINTMENT_H

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

#include <stdio.h>
#include <string.h>

#include "AppointmentMgmt.h"

/* ===========================================================
   3. EDIT APPOINTMENT  (only Scheduled appointments)
   =========================================================== */
static inline void editAppointment(Appointment *appts, int total,
                                   Doctor doctors[], int doctorCount) {
    while (getchar() != '\n');

    printf("\n--- Edit Appointment (enter 0 at any prompt to go back) ---\n");

    char id[20];
    printf("Enter appointment ID to edit: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    if (strcasecmp(id, "0") == 0 || id[0] == '\0') { printf("Going back.\n"); return; }

    /* Find appointment by ID */
    int idx = -1;
    for (int i = 0; i < total; i++) {
        if (strcmp(appts[i].appointmentID, id) == 0) { idx = i; break; }
    }

    if (idx == -1) { printf("Appointment '%s' not found.\n", id); return; }
    if (strcmp(appts[idx].status, "Scheduled") != 0) {
        printf("Only Scheduled appointments can be edited.\n"); return;
    }

    printf("\nCurrent details:\n");
    printAppointment(&appts[idx]);

    printf("\nWhat to edit?\n");
    printf("1. Patient Name\n");
    printf("2. Date\n");
    printf("3. Doctor (and Date)\n");
    printf("4. Type (Regular/PWD/Senior/Pregnant)\n");
    printf("0. Go back\n");
    printf("Choice: ");

    int opt;
    if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    switch (opt) {
        case 0: printf("Going back.\n"); return;

        case 1:    /* Edit patient name */
            printf("New patient name (0 to go back): ");
            fgets(appts[idx].patientName, sizeof(appts[idx].patientName), stdin);
            appts[idx].patientName[strcspn(appts[idx].patientName, "\n")] = '\0';
            if (strcmp(appts[idx].patientName, "0") == 0 ||
                appts[idx].patientName[0] == '\0') {
                printf("Going back.\n"); return;
            }
            break;

        case 2: {  /* Edit date only */
            char newDate[11];
            printf("New date (0 to go back): ");
            if (!pickDate(newDate)) return;
           
            /* Check doctor is still free on the new date */
            if (!canBookDoctor(appts, total, appts[idx].doctorName, newDate)) {
                printf("Dr. %s is not available on %s.\n",
                       appts[idx].doctorName, newDate);
                return;
            }
            strcpy(appts[idx].date, newDate);
            break;
        }

        case 3: {  /* Change doctor (also picks new date) */
            char newDate[11];
            printf("New date for re-booking (0 to go back): ");
            if (!pickDate(newDate)) return;

            int avail[MAX_DOCTORS], ac = 0;
            printf("Available doctors on %s:\n", newDate);
            for (int i = 0; i < doctorCount; i++) {
                if (canBookDoctor(appts, total, doctors[i].name, newDate)) {
                    printf("  %d. Dr. %s\n", ac + 1, doctors[i].name);
                    avail[ac++] = i;
                }
            }
            if (ac == 0) { printf("No doctors available.\n"); return; }

            printf("  0. Go back\n");
            int choice;
            printf("Select (0-%d): ", ac);
            if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); return; }
            if (choice == 0) { printf("Going back.\n"); while (getchar() != '\n'); return; }
            if (choice < 1 || choice > ac) {
                printf("Invalid.\n"); while (getchar() != '\n'); return;
            }

            strcpy(appts[idx].doctorName, doctors[avail[choice - 1]].name);
            strcpy(appts[idx].date, newDate);
            break;
        }

        case 4: {  /* Change type */
        const char *types[] = {"Pregnant", "Senior", "PWD", "Regular"};
        int typeChoice;
        while (1) {
            printf("\nNew appointment type:\n");
            for (int i = 0; i < 4; i++)
                printf("  %d. %s\n", i + 1, types[i]);
            printf("  0. Go back\n");
            printf("Choice: ");

            if (scanf("%d", &typeChoice) != 1) { while (getchar() != '\n'); continue; }
            while (getchar() != '\n');

            if (typeChoice == 0) { printf("Going back.\n"); return; }
            if (typeChoice >= 1 && typeChoice <= 4) {
                strcpy(appts[idx].type, types[typeChoice - 1]);
                break;
            }
            printf("Invalid choice.\n");
        }
        break;
        }

        default: printf("Invalid option.\n"); return;
    }

    saveAppointments(appts, total);
    printf("\nAppointment updated successfully!\n");
    printAppointment(&appts[idx]);
}

#endif /* EDIT_APPOINTMENT_H */