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
#include "QueueManagement.h" // for checking if a patient checked in on the appointment date to mark as Completed or Missed

#include "BookAppointment.h"
#include "ViewAppointment.h"
#include "EditAppointment.h"
#include "CancelAppointment.h"


#define MAX_APPOINTMENTS 100

/* ---- Appointment struct ---- */
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

#endif /* APPOINTMENTMGMT_H */
