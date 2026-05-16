#ifndef CANCEL_APPOINTMENT_H
#define CANCEL_APPOINTMENT_H

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

#include <stdio.h>
#include <string.h>

#include "../core/AppointmentMgmt.h"

/* ===========================================================
   4. CANCEL APPOINTMENT
   =========================================================== */
static inline void cancelAppointment(Appointment *appts, int total) {
    while (getchar() != '\n');

    printf("\n--- Cancel Appointment (enter 0 to go back) ---\n");

    char id[20];
    printf("Enter appointment ID to cancel: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    if (strcasecmp(id, "0") == 0 || id[0] == '\0') { printf("Going back.\n"); return; }

    for (int i = 0; i < total; i++) {
        if (strcasecmp(appts[i].appointmentID, id) == 0) {

            if (strcasecmp(appts[i].status, "Cancelled") == 0) {
                printf("This appointment is already cancelled.\n"); return;
            }
            if (strcmp(appts[i].status, "Scheduled") != 0) {
            printf("Only Scheduled appointments can be cancelled.\n"); return;
            }

            printf("\nAppointment details:\n");
            printAppointment(&appts[i]);
            printf("\nConfirm cancellation? (y/n, 0 to go back): ");

            char c;
            scanf(" %c", &c);
            while (getchar() != '\n');
            if (c == '0') { printf("Going back.\n"); return; }
            if (c == 'y' || c == 'Y') {
                strcpy(appts[i].status, "Cancelled");
                saveAppointments(appts, total);
                printf("Appointment '%s' cancelled successfully.\n", id);
            } else {
                printf("Cancellation aborted.\n");
            }
            return;
        }
    }

    printf("Appointment '%s' not found.\n", id);
}

#endif // CANCEL_APPOINTMENT_H