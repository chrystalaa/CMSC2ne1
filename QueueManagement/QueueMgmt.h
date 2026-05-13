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
#include "DoctorManagement.h"

/* ---- Queue limits ---- */
#define MAX_QUEUE        10
#define MAX_APPTS_LOCAL 100

/*
 * Per-doctor FIFO queues (Unit 6: Pointers, Unit 9.2: 2-D Arrays)
 * queueSlots[d][p] = appointment ID of position p for doctor d
 */
static char queueSlots[MAX_DOCTORS][MAX_QUEUE][20];
static int  queueCount[MAX_DOCTORS];   /* current queue length per doctor */
static int  doctorBusy[MAX_DOCTORS];   /* 1 = a patient is in the room    */

/* ---- Compact appointment struct for queue use ---- */
struct QAppointment {
    char appointmentID[20];
    char patientName[50];
    char doctorName[50];
    char date[20];
    char status[20];
};

/* ===========================================================
   HELPER: Look up a Scheduled appointment from appointments.txt
   Fills *out and sets *docIdx to the matching doctor array index.
   Returns 1 on success, 0 if not found / already done.
   =========================================================== */
static inline int verifyAndGetAppt(const char *searchID,
                                   struct QAppointment *out,
                                   Doctor doctors[], int doctorCount,
                                   int *docIdx) {
    FILE *fp = fopen("appointments.txt", "r");
    if (!fp) return 0;

    char line[300];
    int  found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\r') continue;

        struct QAppointment tmp;
        if (sscanf(line, "%19[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^\n]",
                   tmp.appointmentID, tmp.patientName,
                   tmp.doctorName, tmp.date, tmp.status) == 5) {

            if (strcmp(tmp.appointmentID, searchID) == 0 &&
                strcmp(tmp.status, "Scheduled") == 0) {

                *out   = tmp;
                *docIdx = -1;

                /* Match doctor name to array index (Unit 5: Arrays) */
                for (int i = 0; i < doctorCount; i++) {
                    if (strstr(doctors[i].name, tmp.doctorName) ||
                        strstr(tmp.doctorName, doctors[i].name)) {
                        *docIdx = i; break;
                    }
                }
                found = 1;
                break;
            }
        }
    }

    fclose(fp);
    return found;
}

/* ===========================================================
   HELPER: Mark a given appointment ID as "Completed" in the file
   =========================================================== */
static inline void markCompleted(const char *targetID) {
    struct QAppointment all[MAX_APPTS_LOCAL];
    int total = 0;

    /* Read all appointments */
    FILE *fp = fopen("appointments.txt", "r");
    if (!fp) return;

    char line[300];
    while (fgets(line, sizeof(line), fp) && total < MAX_APPTS_LOCAL) {
        if (line[0] == '\n' || line[0] == '\r') continue;
        if (sscanf(line, "%19[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^\n]",
                   all[total].appointmentID, all[total].patientName,
                   all[total].doctorName, all[total].date,
                   all[total].status) == 5) {

            /* Update status if this is the target */
            if (strcmp(all[total].appointmentID, targetID) == 0)
                strcpy(all[total].status, "Completed");
            total++;
        }
    }
    fclose(fp);

    /* Rewrite file with updated status */
    fp = fopen("appointments.txt", "w");
    if (!fp) return;
    for (int i = 0; i < total; i++)
        fprintf(fp, "%s|%s|%s|%s|%s\n",
                all[i].appointmentID, all[i].patientName,
                all[i].doctorName, all[i].date, all[i].status);
    fclose(fp);
}

/* ===========================================================
   1. CHECK-IN PATIENT  (adds to the doctor's queue)
   =========================================================== */
static inline void checkInPatient(Doctor doctors[], int doctorCount) {
    while (getchar() != '\n');

    printf("\n--- Check-in Patient (enter 0 to go back) ---\n");

    char inputID[20];
    printf("Enter Appointment ID (e.g. D1-0001): ");
    fgets(inputID, sizeof(inputID), stdin);
    inputID[strcspn(inputID, "\n")] = '\0';

    if (strcmp(inputID, "0") == 0 || inputID[0] == '\0') {
        printf("Going back.\n"); return;
    }

    struct QAppointment appt;
    int docIdx = -1;

    /* Validate appointment exists, is Scheduled, and find its doctor */
    if (!verifyAndGetAppt(inputID, &appt, doctors, doctorCount, &docIdx)) {
        printf("Appointment not found, already completed/cancelled, or invalid ID.\n");
        return;
    }

    if (docIdx < 0 || docIdx >= doctorCount) {
        printf("Could not match doctor in the system.\n"); return;
    }

    if (queueCount[docIdx] >= MAX_QUEUE) {
        printf("Queue for Dr. %s is full. Please wait.\n", doctors[docIdx].name);
        return;
    }

    /* Prevent duplicate check-ins (Unit 4: Loops) */
    for (int j = 0; j < queueCount[docIdx]; j++) {
        if (strcmp(queueSlots[docIdx][j], inputID) == 0) {
            printf("Already checked in (position %d in queue).\n", j + 1);
            return;
        }
    }

    /* Enqueue: append ID to the doctor's queue */
    strcpy(queueSlots[docIdx][queueCount[docIdx]], inputID);
    queueCount[docIdx]++;

    printf("Check-in successful!\n");
    printf("  Patient : %s\n",     appt.patientName);
    printf("  Doctor  : Dr. %s\n", doctors[docIdx].name);
    printf("  Position: #%d in queue\n", queueCount[docIdx]);
}

/* ===========================================================
   2. VIEW QUEUE STATUS  (live display for all doctors)
   =========================================================== */
static inline void viewQueueStatus(Doctor doctors[], int doctorCount) {
    printf("\n==========================================\n");
    printf("       LIVE CLINIC QUEUE MONITOR          \n");
    printf("==========================================\n");

    for (int i = 0; i < doctorCount; i++) {
        printf("Dr. %-22s | ", doctors[i].name);

        if (queueCount[i] == 0) {
            printf("[No patients in queue]\n");
        } else {
            for (int j = 0; j < queueCount[i]; j++) {
                if (j == 0)
                    /* First patient: show whether they're in the room or next */
                    printf("%s %s  ",
                           queueSlots[i][j],
                           doctorBusy[i] ? "(IN ROOM)" : "(NEXT UP)");
                else
                    printf("[%s] ", queueSlots[i][j]);
            }
            printf("\n");
        }
    }
    printf("==========================================\n");
}

/* ===========================================================
   3. PROCESS NEXT PATIENT  (doctor control panel)
   =========================================================== */
static inline void processNextPatient(Doctor doctors[], int doctorCount) {
    viewQueueStatus(doctors, doctorCount);

    printf("Enter doctor number to manage (1-%d, 0 = go back): ", doctorCount);
    int doc;
    if (scanf("%d", &doc) != 1) { while (getchar() != '\n'); return; }
    if (doc == 0) { printf("Going back.\n"); while (getchar() != '\n'); return; }

    doc--;  /* convert 1-based input to 0-based index */
    if (doc < 0 || doc >= doctorCount) {
        printf("Invalid number.\n"); while (getchar() != '\n'); return;
    }

    printf("\nDr. %s's Control Panel\n", doctors[doc].name);
    printf("Queue size: %d\n", queueCount[doc]);
    printf("1. Call Next Patient / Start Treatment\n");
    printf("2. Mark Current Patient as Done\n");
    printf("0. Go back\n");
    printf("Choice: ");

    int action;
    if (scanf("%d", &action) != 1) { while (getchar() != '\n'); return; }

    switch (action) {
        case 0:
            printf("Going back.\n");
            break;

        case 1:   /* Call the first patient in queue into the room */
            if (queueCount[doc] == 0) { printf("Queue is empty.\n"); break; }
            doctorBusy[doc] = 1;
            printf("Patient %s is now in the room.\n", queueSlots[doc][0]);
            break;

        case 2:   /* Dequeue: mark done, shift queue left */
            if (queueCount[doc] == 0) { printf("Queue is empty.\n"); break; }
            markCompleted(queueSlots[doc][0]);
            printf("Patient %s marked as Completed.\n", queueSlots[doc][0]);

            /* Shift remaining patients forward (Unit 5: Arrays) */
            for (int i = 0; i < queueCount[doc] - 1; i++)
                strcpy(queueSlots[doc][i], queueSlots[doc][i + 1]);
            queueCount[doc]--;
            doctorBusy[doc] = 0;

            if (queueCount[doc] > 0)
                printf("Next up: %s\n", queueSlots[doc][0]);
            else
                printf("Queue is now empty.\n");
            break;

        default:
            printf("Invalid option.\n");
    }
}

/* ===========================================================
   QUEUE MANAGEMENT MENU
   =========================================================== */
static inline void QueueManagement(Doctor doctors[], int doctorCount) {
    int opt;
    while (1) {
        printf("\n==========================================\n");
        printf("        QUEUE MANAGEMENT MENU             \n");
        printf("==========================================\n");
        printf("1. Check-in Patient\n");
        printf("2. View Queue Status\n");
        printf("3. Process Next Patient (Doctor)\n");
        printf("0. Back to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); continue; }

        switch (opt) {
            case 1: checkInPatient(doctors, doctorCount);     break;
            case 2: viewQueueStatus(doctors, doctorCount);    break;
            case 3: processNextPatient(doctors, doctorCount); break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

#endif /* QUEUEMGMT_H */
