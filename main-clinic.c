/*
 * main-clinic.c
 * ============================================================
 * Clinic Management System — Main Entry Point
 *
 * Compile:
 *   gcc main-clinic.c -o clinic -Wall
 *
 * First run: seeds 5 placeholder doctors if doctors.txt is empty.
 * Navigation: enter 0 at any sub-prompt to go back one level.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DoctorManagement.h"
#include "PatientMgmt.h"
#include "AppointmentMgmt.h"
#include "QueueMgmt.h"

/* Clear the terminal (cross-platform) */
static void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Top banner shown on every main-menu loop */
static void printBanner(void) {
    printf("\n");
    printf("  ==========================================\n");
    printf("        CLINIC MANAGEMENT SYSTEM           \n");
    printf("  ==========================================\n");
    printf("  Tip: enter 0 at any prompt to go back.   \n");
    printf("  ==========================================\n");
}

int main(void) {
    Doctor doctors[MAX_DOCTORS];
    int    doctorCount = loadDoctors(doctors);   /* load from doctors.txt */

    /* First-run seed: populate 5 placeholder doctors if none exist */
    if (doctorCount == 0)
        doctorCount = seedDefaultDoctors(doctors);

    int choice;

    while (1) {
        clearScreen();
        printBanner();

        printf("\n");
        printf("  1. Appointment Management\n");
        printf("  2. Queue Management\n");
        printf("  3. Patient Management\n");
        printf("  4. Doctor Management\n");
        printf("  5. Exit\n");
        printf("\n  Select option: ");

        /* Safely read menu choice; flush on bad input */
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: AppointmentManagement(doctors, doctorCount); break;
            case 2: QueueManagement(doctors, doctorCount);       break;
            case 3: PatientManagement();                         break;
            case 4: DoctorManagement(doctors, &doctorCount);     break;
            case 5: printf("\n  Goodbye!\n\n"); return 0;
            default: printf("  Invalid option. Please choose 1-5.\n");
        }
    }
}
