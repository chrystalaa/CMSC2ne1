/*
 * DoctorManagement.h
 * ============================================================
 * Handles everything related to doctors:
 *   - Struct definition + file I/O (load / save)
 *   - Seed default doctors on first run
 *   - View, view schedule, add, delete
 *
 * Concepts used: structs, file handling, arrays, functions,
 *                pointers, strings (CMSC21 Units 1-9)
 * ============================================================
 */

#ifndef DOCTORMANAGEMENT_H
#define DOCTORMANAGEMENT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AddDoctor.h"

/* ---- Constants ---- */
#define MAX_DOCTORS  10
#define DOCTOR_FILE  "doctors.txt"

/* ---- Doctor struct (Unit 8: Structures) ---- */
typedef struct {
    int  id;
    char name[50];
    char specialty[50];
    int  isAvailable;   /* 1 = available, 0 = unavailable */
} Doctor;

/* ===========================================================
   FILE I/O  (Unit 9: File Handling)
   =========================================================== */

/* Save the entire doctors array to DOCTOR_FILE (pipe-delimited) */
static inline void saveDoctors(Doctor doctors[], int count) {
    FILE *fp = fopen(DOCTOR_FILE, "w");
    if (!fp) { printf("Error: could not save doctors.\n"); return; }

    for (int i = 0; i < count; i++)
        fprintf(fp, "%d|%s|%s|%d\n",
                doctors[i].id,
                doctors[i].name,
                doctors[i].specialty,
                doctors[i].isAvailable);

    fclose(fp);
}

/* Load doctors from DOCTOR_FILE into the array; returns count loaded */
static inline int loadDoctors(Doctor doctors[]) {
    FILE *fp = fopen(DOCTOR_FILE, "r");
    if (!fp) return 0;

    int   count = 0;
    char  line[200];

    while (fgets(line, sizeof(line), fp) && count < MAX_DOCTORS) {
        /* Parse pipe-delimited fields */
        if (sscanf(line, "%d|%49[^|]|%49[^|]|%d",
                   &doctors[count].id,
                    doctors[count].name,
                    doctors[count].specialty,
                   &doctors[count].isAvailable) == 4)
            count++;
    }

    fclose(fp);
    return count;
}

/* ===========================================================
   SEED DEFAULT DOCTORS  (called from main if file is empty)
   =========================================================== */
static inline int seedDefaultDoctors(Doctor doctors[]) {
    /* Only seed when file is missing or empty */
    FILE *check = fopen(DOCTOR_FILE, "r");
    if (check) {
        fseek(check, 0, SEEK_END);
        long sz = ftell(check);
        fclose(check);
        if (sz > 0) return 0;   /* already has data — skip */
    }

    /* Placeholder doctor data */
    const char *names[] = { "Kevin Sinugbuhan", "Matthew Honorario",
                             "Chrystel Mendoza",  "Hannah Narca",
                             "Samuel Niebla" };
    const char *specs[] = { "Cardiology",      "General Practice",
                             "Pediatrics",      "Orthopedics",
                             "Dermatology" };
    int n = 5;

    for (int i = 0; i < n; i++) {
        doctors[i].id = i + 1;
        strncpy(doctors[i].name,      names[i], 49); doctors[i].name[49]      = '\0';
        strncpy(doctors[i].specialty, specs[i],  49); doctors[i].specialty[49] = '\0';
        doctors[i].isAvailable = 1;
    }

    saveDoctors(doctors, n);
    printf("[System] Seeded %d default doctors.\n", n);
    return n;
}
/* ===========================================================
   DOCTOR MANAGEMENT MENU
   =========================================================== */
static inline void DoctorManagement(Doctor doctors[], int *count) {
    int opt;
    while (1) {
        printf("\n==========================================\n");
        printf("         DOCTOR MANAGEMENT MENU          \n");
        printf("==========================================\n");
        printf("1. View Doctors\n");
        printf("2. View Doctor Schedule\n");
        printf("3. Add Doctor\n");
        printf("4. Delete Doctor\n");
        printf("0. Back to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); continue; }

        switch (opt) {
            case 1: viewDoctors(doctors, *count);        break;
            case 2: viewDoctorSchedule(doctors, *count); break;
            case 3: addDoctor(doctors, count);           break;
            case 4: deleteDoctor(doctors, count);        break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

#endif /* DOCTORMANAGEMENT_H */
