#ifndef VIEW_APPOINTMENT_H
#define VIEW_APPOINTMENT_H

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

#include <stdio.h>
#include <string.h>
#include "../core/AppointmentMgmt.h"


/* ===========================================================
   2. VIEW APPOINTMENTS  (with filter: All / Upcoming / Past)
   =========================================================== */
static inline void viewAppointments(Appointment *appts, int total) {
    int opt;
    while (getchar() != '\n');

    printf("\n--- View Appointments ---\n");
    printf("1. All\n");
    printf("2. Scheduled\n");
    printf("3. Completed\n");
    printf("4. Cancelled\n");
    printf("5. Missed\n");
    printf("6. Search by ID\n");
    printf("0. Go back\n");
    printf("Select filter: ");
    if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); return; }
    if (opt == 0) { printf("Going back.\n"); return; }
    
    char searchID[9] = ""; /* Init for option 4 search */
    switch (opt) {
        case 1: printf("Showing all appointments.\n"); break;
        case 2: printf("Showing only scheduled appointments.\n"); break;
        case 3: printf("Showing only completed appointments.\n"); break;
        case 4: printf("Showing only cancelled appointments.\n"); break;
        case 5: printf("Showing only missed appointments.\n"); break;
        case 6: { int found = 0;

                printf("Enter appointment ID to search: ");
                scanf("%8s", searchID);
                while (getchar() != '\n');
                for (int i = 0; i < total; i++) {
                    if (strcasecmp(appts[i].appointmentID, searchID) == 0) {
                        found = 1; break; }
                }
                if (!found) {printf("No appointment found with ID '%s'.\n", searchID); return;}
                break;
        }
        default:
            printf("Invalid option. Returning to menu.\n");
            return;
    }

    /*Print header formatting*/
    printf("\n%-14s %-22s %-22s %-12s %-12s %-12s\n", "ID", "Patient", "Doctor", "Date", "Type", "Status");
    printf("---------------------------------------------------------------------------------------------------\n");

    int printed = 0;
    for (int i = 0; i < total; i++) {
        /* Determine if this record matches the chosen filter */
        int show = (opt == 1) ||
                   (opt == 2 && strcmp(appts[i].status, "Scheduled")  == 0) ||
                   (opt == 3 && strcmp(appts[i].status, "Completed")  == 0) ||
                   (opt == 4 && strcmp(appts[i].status, "Cancelled")  == 0) ||
                   (opt == 5 && strcmp(appts[i].status, "Missed")  == 0) ||
                   (opt == 6 && strcasecmp(appts[i].appointmentID, searchID) == 0);

        if (show) {
            printf("%-14s %-22s %-22s %-12s %-12s %-12s\n",
                   appts[i].appointmentID, appts[i].patientName,
                   appts[i].doctorName,    appts[i].date,
                   appts[i].type,          appts[i].status);
            printed++;
        }
    }

    if (printed == 0) printf("No appointments to display.\n");
    printf("----------------------------------------------------------------------\n");
    printf("Total shown: %d\n", printed);
}

#endif /* VIEW_APPOINTMENT_H */