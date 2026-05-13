/*
 * AppointmentMgmt.h
 * ============================================================
 * Handles appointment CRUD stored in appointments.txt:
 *   Book, View (filtered), Edit, Cancel
 *
 * appointments.txt format (pipe-delimited, one line per record):
 *   <ApptID>|<PatientName>|<DoctorName>|<Date>|<Status>
 *   Status values: Scheduled | Cancelled | Completed
 *
 * Concepts used: structs, file handling, arrays, strings,
 *                pointers, functions  (CMSC21 Units 1-9)
 * ============================================================
 */

#ifndef APPOINTMENTMGMT_H
#define APPOINTMENTMGMT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DoctorManagement.h"
#include "BookAppointment.h"
#include "ViewAppointment.h"
#include "EditAppointment.h"
#include "CancelAppointment.h"


#define MAX_APPOINTMENTS 100

/* ---- Appointment struct (Unit 8) ---- */
typdef struct {
    char appointmentID[8];
    char patientName[50];
    char doctorName[50];
    char date[11];
    char status[20];   /* Scheduled 9 | Cancelled | Completed */
}  Appointment ;

/* ===========================================================
   FILE I/O  (Unit 9)
   =========================================================== */

/* Overwrite appointments.txt with the current in-memory array */
static inline void saveAppointments(struct Appointment *appts, int total) {
    FILE *fp = fopen("appointments.txt", "w");
    if (!fp) { printf("Error saving appointments.\n"); return; }

    for (int i = 0; i < total; i++)
        fprintf(fp, "%s|%s|%s|%s|%s\n",
                appts[i].appointmentID, appts[i].patientName,
                appts[i].doctorName,    appts[i].date,
                appts[i].status);

    fclose(fp);
}

/* Load appointments from file; returns count. Opens with "a+" to create if missing */
static inline int loadAppointments(struct Appointment *appts) {
    FILE *fp = fopen("appointments.txt", "a+");  /* create if not exists */
    if (!fp) return 0;
    rewind(fp);

    int  total = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp) && total < MAX_APPOINTMENTS) {
        if (line[0] == '\n' || line[0] == '\r') continue;  /* skip blank lines */
        if (sscanf(line, "%7[^|]|%49[^|]|%49[^|]|%10[^|]|%19[^\n]",
                   appts[total].appointmentID,
                   appts[total].patientName,
                   appts[total].doctorName,
                   appts[total].date,
                   appts[total].status) == 5)
            total++;
    }

    fclose(fp);
    return total;
}

/* ===========================================================
   HELPER FUNCTIONS
   =========================================================== */

/*
 * Returns 1 if the given doctor has NO Scheduled appointment on 'date'.
 * Used to prevent double-booking the same doctor on the same day.
 */
static inline int isDoctorAvailable(struct Appointment *appts, int total,
                                    const char *doctorName, const char *date) {
    for (int i = 0; i < total; i++) {
        if (strcmp(appts[i].doctorName, doctorName) == 0 &&
            strcmp(appts[i].date,       date)       == 0 &&
            strcmp(appts[i].status,     "Scheduled") == 0)
            return 0;   /* already booked */
    }
    return 1;
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
        if (strstr(doctors[i].name, doctorName)) { docIdx = i + 1; break; }
    }

    /* Read persistent counter */
    int counter = 1;
    FILE *cf = fopen("appt_counter.txt", "r");
    if (cf) { fscanf(cf, "%d", &counter); fclose(cf); }

    sprintf(id, "D%d-%04d", docIdx, counter);

    /* Increment and save counter for next booking */
    cf = fopen("appt_counter.txt", "w");
    if (cf) { fprintf(cf, "%d", counter + 1); fclose(cf); }
}

/* Pretty-print one appointment */
static inline void printAppointment(struct Appointment *a) {
    printf("  ID     : %s\n", a->appointmentID);
    printf("  Patient: %s\n", a->patientName);
    printf("  Doctor : %s\n", a->doctorName);
    printf("  Date   : %s\n", a->date);
    printf("  Status : %s\n", a->status);
}

/* ===========================================================
   1. BOOK APPOINTMENT
   =========================================================== */
static inline void bookAppointment(struct Appointment *appts, int *total,
                                   Doctor doctors[], int doctorCount) {
    if (*total >= MAX_APPOINTMENTS) {
        printf("Cannot book: maximum capacity (%d) reached.\n", MAX_APPOINTMENTS);
        return;
    }

    struct Appointment na;
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
        if (isDoctorAvailable(appts, *total, doctors[i].name, na.date)) {
            printf("  %d. Dr. %-22s (%s)\n",
                   availCount + 1, doctors[i].name, doctors[i].specialty);
            available[availCount++] = i;
        }
    }

    if (availCount == 0) {
        printf("No doctors available on %s.\n", na.date); return;
    }

    printf("  0. Go back\n");
    printf("Select doctor (0-%d): ", availCount);
    int choice;
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

/* ===========================================================
   2. VIEW APPOINTMENTS  (with filter: All / Upcoming / Past)
   =========================================================== */
static inline void viewAppointments(struct Appointment *appts, int total) {
    int opt;
    while (getchar() != '\n');

    printf("\n--- View Appointments ---\n");
    printf("1. All\n");
    printf("2. Upcoming (Scheduled)\n");
    printf("3. Past (Completed / Cancelled)\n");
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

/* ===========================================================
   3. EDIT APPOINTMENT  (only Scheduled appointments)
   =========================================================== */
static inline void editAppointment(struct Appointment *appts, int total,
                                   Doctor doctors[], int doctorCount) {
    while (getchar() != '\n');

    printf("\n--- Edit Appointment (enter 0 at any prompt to go back) ---\n");

    char id[20];
    printf("Enter appointment ID to edit: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    if (strcmp(id, "0") == 0 || id[0] == '\0') { printf("Going back.\n"); return; }

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
    printf("0. Go back\n");
    printf("Choice: ");

    int opt;
    if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    switch (opt) {
        case 0: printf("Going back.\n"); return;

        case 1:   /* Edit patient name */
            printf("New patient name (0 to go back): ");
            fgets(appts[idx].patientName, sizeof(appts[idx].patientName), stdin);
            appts[idx].patientName[strcspn(appts[idx].patientName, "\n")] = '\0';
            if (strcmp(appts[idx].patientName, "0") == 0 ||
                appts[idx].patientName[0] == '\0') {
                printf("Going back.\n"); return;
            }
            break;

        case 2: {  /* Edit date only */
            char newDate[20];
            printf("New date (YYYY-MM-DD, 0 to go back): ");
            fgets(newDate, sizeof(newDate), stdin);
            newDate[strcspn(newDate, "\n")] = '\0';
            if (strcmp(newDate, "0") == 0 || newDate[0] == '\0') {
                printf("Going back.\n"); return;
            }
            /* Check doctor is still free on the new date */
            if (!isDoctorAvailable(appts, total, appts[idx].doctorName, newDate)) {
                printf("Dr. %s is not available on %s.\n",
                       appts[idx].doctorName, newDate);
                return;
            }
            strcpy(appts[idx].date, newDate);
            break;
        }

        case 3: {  /* Change doctor (also picks new date) */
            char newDate[20];
            printf("New date for re-booking (YYYY-MM-DD, 0 to go back): ");
            fgets(newDate, sizeof(newDate), stdin);
            newDate[strcspn(newDate, "\n")] = '\0';
            if (strcmp(newDate, "0") == 0 || newDate[0] == '\0') {
                printf("Going back.\n"); return;
            }

            int avail[MAX_DOCTORS], ac = 0;
            printf("Available doctors on %s:\n", newDate);
            for (int i = 0; i < doctorCount; i++) {
                if (isDoctorAvailable(appts, total, doctors[i].name, newDate)) {
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

        default: printf("Invalid option.\n"); return;
    }

    saveAppointments(appts, total);
    printf("\nAppointment updated successfully!\n");
    printAppointment(&appts[idx]);
}

/* ===========================================================
   4. CANCEL APPOINTMENT
   =========================================================== */
static inline void cancelAppointment(struct Appointment *appts, int total) {
    while (getchar() != '\n');

    printf("\n--- Cancel Appointment (enter 0 to go back) ---\n");

    char id[20];
    printf("Enter appointment ID to cancel: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';
    if (strcmp(id, "0") == 0 || id[0] == '\0') { printf("Going back.\n"); return; }

    for (int i = 0; i < total; i++) {
        if (strcmp(appts[i].appointmentID, id) == 0) {

            if (strcmp(appts[i].status, "Cancelled") == 0) {
                printf("This appointment is already cancelled.\n"); return;
            }

            printf("\nAppointment details:\n");
            printAppointment(&appts[i]);
            printf("\nConfirm cancellation? (y/n, 0 to go back): ");

            char c;
            scanf(" %c", &c);
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

/* ===========================================================
   APPOINTMENT MANAGEMENT MENU
   =========================================================== */
static inline void AppointmentManagement(Doctor doctors[], int doctorCount) {
    struct Appointment appts[MAX_APPOINTMENTS];
    int total = loadAppointments(appts);   /* fresh load from file */

    int opt;
    while (1) {
        printf("\n==========================================\n");
        printf("      APPOINTMENT MANAGEMENT MENU         \n");
        printf("==========================================\n");
        printf("Loaded appointments: %d\n", total);
        printf("------------------------------------------\n");
        printf("1. Book an Appointment\n");
        printf("2. View Appointments\n");
        printf("3. Edit an Appointment\n");
        printf("4. Cancel an Appointment\n");
        printf("0. Back to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); continue; }

        switch (opt) {
            case 1: bookAppointment(appts, &total, doctors, doctorCount); break;
            case 2: viewAppointments(appts, total);                       break;
            case 3: editAppointment(appts, total, doctors, doctorCount);  break;
            case 4: cancelAppointment(appts, total);                      break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

#endif /* APPOINTMENTMGMT_H */
