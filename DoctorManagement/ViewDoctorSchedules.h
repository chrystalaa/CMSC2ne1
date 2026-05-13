#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DoctorManagement.h"

/* ===========================================================
   VIEW DOCTOR SCHEDULE  (reads appointments.txt for a doctor)
   =========================================================== */
static inline void viewDoctorSchedule(Doctor *doctors, int count) {
    (void)doctors;  /* doctors[] not directly needed; we search by name in file */
    if (count == 0) { printf("No doctors on record.\n"); return; }

    while (getchar() != '\n');   /* flush leftover newline */

    char searchName[50];
    printf("Enter doctor name to view schedule (0 to go back): ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    if (strcmp(searchName, "0") == 0 || searchName[0] == '\0') {
        printf("Going back.\n"); return;
    }

    FILE *fp = fopen("appointments.txt", "r");
    if (!fp) { printf("No appointments on record.\n"); return; }

    printf("\n--- Schedule for Dr. %s ---\n", searchName);
    printf("%-20s %-14s %-12s %-12s\n",
           "Patient", "Appt ID", "Date", "Status");
    printf("------------------------------------------------------------\n");

    char line[300];
    int  found = 0;

    while (fgets(line, sizeof(line), fp)) {
        char id[20], patient[50], doctor[50], date[20], status[20];
        if (sscanf(line, "%19[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^\n]",
                   id, patient, doctor, date, status) == 5) {
            if (strstr(doctor, searchName)) {
                printf("%-20s %-14s %-12s %-12s\n", patient, id, date, status);
                found = 1;
            }
        }
    }

    if (!found) printf("No appointments found for Dr. %s.\n", searchName);
    printf("------------------------------------------------------------\n");
    fclose(fp);
}