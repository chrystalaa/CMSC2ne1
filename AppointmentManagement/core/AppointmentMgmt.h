/*
 * AppointmentMgmt.h
 * ============================================================
 * Handles appointment CRUD stored in appointments.txt:
 *   Book, View (filtered), Edit, Cancel
 *
 * appointments.txt format (pipe-delimited, one line per record):
 *   <ApptID>|<PatientName>|<DoctorName>|<Date>|<Type>|<Status>
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
#include "../../DoctorManagement/DoctorMgmt.h"

#include "ApptFileHandling.h"


/* ===========================================================
   APPOINTMENT MANAGEMENT MENU
   =========================================================== */
static inline void AppointmentManagement(Doctor doctors[], int doctorCount) {
    Appointment appts[MAX_APPOINTMENTS];
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

        if (scanf("%d", &opt) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            if (c == EOF) return;  // or break
            continue;
        }

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
