/*
 * AppointmentMgmt.h
 * ============================================================
 * Handles appointment CRUD stored in appointments.txt:
 *   Book, View (filtered), Edit, Cancel
 *
 * appointments.txt format (pipe-delimited, one line per record):
 *   <ApptID>|<PatientName>|<DoctorName>|<Date>|<Status>|<Type>
 *   Status values: Scheduled | Cancelled | Completed | Missed
 *
 * Concepts used: structs, file handling, arrays, strings,
 *                pointers, functions
 * ============================================================
 */

#ifndef APPOINTMENTMGMT_H
#define APPOINTMENTMGMT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "DoctorManagement.h"
#include "QueueManagement.h"

#include "BookAppointment.h"
#include "ViewAppointment.h"
#include "EditAppointment.h"
#include "CancelAppointment.h"


#define MAX_APPOINTMENTS 100

/* ---- Appointment struct (Unit 8) ---- */
typedef struct {
    char appointmentID[8];
    char patientName[50];
    char doctorName[50];
    char date[11];
    char status[15];   /* Scheduled | Cancelled | Completed | Missed */
    char type[20]; /* Pregnant | Senior | PWD | Regular */
}  Appointment ;


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




/* ===========================================================
   FILE I/O
   =========================================================== */

/* Overwrite appointments.txt with the current in-memory array */
static inline void saveAppointments(Appointment *appts, int total) {
    FILE *fp = fopen("appointments.txt", "w");
    if (!fp) { printf("Error saving appointments.\n"); return; }

    for (int i = 0; i < total; i++)
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                appts[i].appointmentID, appts[i].patientName,
                appts[i].doctorName,    appts[i].date,
                appts[i].status,        appts[i].type);

    fclose(fp);
}

/* Load appointments from file; returns count. Opens with "a+" to create if missing */
static inline int loadAppointments(Appointment *appts) {
    FILE *fp = fopen("appointments.txt", "a+");  /* create if not exists */
    if (!fp) return 0;
    rewind(fp);

    int  total = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp) && total < MAX_APPOINTMENTS) {
        if (line[0] == '\n' || line[0] == '\r'|| line[0] == '\0') 
            continue;  /* skip blank lines */
        int result = sscanf(line, "%7[^|]|%49[^|]|%49[^|]|%10[^|]|%14[^|]|%19[^\n]",
                            appts[total].appointmentID,     appts[total].patientName,
                            appts[total].doctorName,        appts[total].date,
                            appts[total].status,            appts[total].type);
        if (result == 6)
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
static inline int canBookDoctor(Appointment *appts, int total,
                                    const char *doctorName, const char *date) {
    int appt_count = 0;

    for (int i = 0; i < total; i++) {
        if (strcmp(appts[i].doctorName, doctorName) == 0 &&
            strcmp(appts[i].date,       date)       == 0 &&
            strcmp(appts[i].status,     "Scheduled") == 0)
            appt_count++; 
    }

    if (appt_count >= 5) {
        printf("Dr. %s's schedule on this date is full. Please choose another date or doctor.\n", doctorName);
        return 0;
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
        if (strcmp(doctors[i].name, doctorName) == 0) { docIdx = i + 1; break; }
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
static inline void printAppointment(Appointment *a) {
    printf("  ID     : %s\n", a->appointmentID);
    printf("  Patient: %s\n", a->patientName);
    printf("  Doctor : %s\n", a->doctorName);
    printf("  Date   : %s\n", a->date);
    printf("  Status : %s\n", a->status);
    printf("  Type   : %s\n", a->type);
}

#endif /* APPOINTMENTMGMT_H */
