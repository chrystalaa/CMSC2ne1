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

/* --- Dual-Queue System Constants & State --- */
static char queueSlots[MAX_DOCTORS][MAX_QUEUE][20];         // Regular Queue
static char priorityQueueSlots[MAX_DOCTORS][MAX_QUEUE][20];  // Priority Queue (PWD, Pregnant, Senior Citizen)
static int  queueCount[MAX_DOCTORS];
static int  priorityQueueCount[MAX_DOCTORS];
static int  doctorBusy[MAX_DOCTORS]; // 0=Empty, 1=Regular Inside, 2=Priority Inside

/* --- Queue Appointment Structure --- */
struct QAppointment {
    char appointmentID[20];
    char patientName[50];
    char doctorName[50];
    char date[20];
    char status[20];
    char type[20]; //(PWD, Pregnant, Senior Citizen)
};

// Subheaders
#include "HelperFunctions.h"
#include "CheckInPatient.h"
#include "ViewQueueStatus.h"
#include "ProcessNextPatient.h"

#endif /* QUEUEMGMT_H */