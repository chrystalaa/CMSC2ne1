#ifndef VIEW_APPOINTMENT_H
#define VIEW_APPOINTMENT_H

#include <stdio.h>
#include <string.h>
#include "AppointmentMgmt.h"


/* ===========================================================
   2. VIEW APPOINTMENTS  (with filter: All / Upcoming / Past)
   =========================================================== */
static inline void viewAppointments(Appointment *appts, int total) {
    int opt;
    while (getchar() != '\n');

    printf("\n--- View Appointments ---\n");
    printf("1. All\n");
    printf("2. Scheduled\n");
    printf("3. Completed / Cancelled \n");
    printf("0. Go back\n");
    printf("Select filter: ");
    if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); return; }
    if (opt == 0) { printf("Going back.\n"); return; }

    printf("\n%-14s %-22s %-22s %-12s %-12s\n",
           "ID", "Patient", "Doctor", "Date", "Status");
    printf("----------------------------------------------------------------------\n");

    int printed = 0;
    for (int i = 0; i < total; i++) {
        /* Determine if this record matches the chosen filter */
        int show = (opt == 1) ||
                   (opt == 2 && strcmp(appts[i].status, "Scheduled")  == 0) ||
                   (opt == 3 && strcmp(appts[i].status, "Scheduled")  != 0);

        if (show) {
            printf("%-14s %-22s %-22s %-12s %-12s\n",
                   appts[i].appointmentID, appts[i].patientName,
                   appts[i].doctorName,    appts[i].date, appts[i].status);
            printed++;
        }
    }

    if (printed == 0) printf("No appointments to display.\n");
    printf("----------------------------------------------------------------------\n");
    printf("Total shown: %d\n", printed);
}

#endif /* VIEW_APPOINTMENT_H */