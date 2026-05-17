/*
 * QueueMgmt.h
 * ============================================================
 * In-memory queue system (session-only; resets on restart):
 *   - Check-in a patient via their Appointment ID
 *   - View live queue per doctor
 *   - Doctor control panel: call next / mark done
 *
 * Queue model: each doctor has a FIFO queue of Appointment IDs
 *   stored in queueSlots[docIdx][position].
 *
 * Concepts used: 2-D arrays, pointers, structs, strings,
 *                file handling, functions, loops (Units 1-9)
 * ============================================================
 */

#ifndef QUEUEMGMT_H
#define QUEUEMGMT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../DoctorManagement/DoctorMgmt.h"

/* ---- Queue limits ---- */
#define MAX_QUEUE        10
#define MAX_APPTS_LOCAL 100

/* --- Dual-Queue System Constants & State --- */
static char queueSlots[MAX_DOCTORS][MAX_QUEUE][9];         // Regular Queue
static char priorityQueueSlots[MAX_DOCTORS][MAX_QUEUE][9];  // Priority Queue (PWD, Pregnant, Senior Citizen)
static int  queueCount[MAX_DOCTORS];
static int  priorityQueueCount[MAX_DOCTORS];
static int  doctorBusy[MAX_DOCTORS]; // 0=Empty, 1=Regular Inside, 2=Priority Inside

/* --- Queue Appointment Structure --- */
struct QAppointment {
    char appointmentID[9];
    char patientName[50];
    char doctorName[50];
    char date[11];
    char type[20]; // (PWD, Pregnant, Senior Citizen, Regular(dikosure))
    char status[20]; // (Scheduled, Checked-in, In-Progress, Completed)
    
};

// Subheaders
#include "FunctionHelpers.h"
#include "CheckInPatient.h"
#include "ViewQueueStatus.h"
#include "ProcessNextPatient.h"


/* ===========================================================
   QUEUE MANAGEMENT MENU
   =========================================================== */
static inline void QueueManagement(Doctor doctors[], int doctorCount) {
    markMissed();  /* end-of-day check on entry */

    int opt;
    while (1) {
        printf("\n==========================================\n");
        printf("        QUEUE MANAGEMENT MENU            \n");
        printf("==========================================\n");
        printf("1. Check-in Patient\n");
        printf("2. View Queue Status\n");
        printf("3. Process Next Patient\n");
        printf("0. Back to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &opt) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            if (c == EOF) return;
            continue;
        }

        switch (opt) {
            case 1: checkInPatient(doctors, doctorCount);   break;
            case 2: viewQueueStatus(doctors, doctorCount);  break;
            case 3: processNextPatient(doctors, doctorCount); break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

#endif /* QUEUEMGMT_H */