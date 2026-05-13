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
   VIEW DOCTORS  (Unit 5: Arrays, Unit 8: Structures)
   =========================================================== */
static inline void viewDoctors(Doctor doctors[], int count) {
    printf("\n==========================================\n");
    printf("              DOCTOR LIST                 \n");
    printf("==========================================\n");

    if (count == 0) {
        printf("  No doctors on record.\n");
    } else {
        printf("%-4s %-22s %-20s %-11s\n",
               "ID", "Name", "Specialty", "Status");
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < count; i++)
            printf("%-4d %-22s %-20s %-11s\n",
                   doctors[i].id,
                   doctors[i].name,
                   doctors[i].specialty,
                   doctors[i].isAvailable ? "Available" : "Unavailable");
    }
    printf("==========================================\n");
}

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

/* ===========================================================
   ADD DOCTOR
   =========================================================== */
static inline void addDoctor(Doctor doctors[], int *count) {
    if (*count >= MAX_DOCTORS) {
        printf("Maximum number of doctors (%d) reached.\n", MAX_DOCTORS);
        return;
    }

    while (getchar() != '\n');   /* flush */

    Doctor d;
    /* Auto-increment ID from last entry */
    d.id = (*count > 0) ? doctors[*count - 1].id + 1 : 1;

    printf("Enter doctor name (0 to go back): ");
    fgets(d.name, sizeof(d.name), stdin);
    d.name[strcspn(d.name, "\n")] = '\0';
    if (strcmp(d.name, "0") == 0 || d.name[0] == '\0') {
        printf("Going back.\n"); return;
    }

    printf("Enter specialty (0 to go back): ");
    fgets(d.specialty, sizeof(d.specialty), stdin);
    d.specialty[strcspn(d.specialty, "\n")] = '\0';
    if (strcmp(d.specialty, "0") == 0) {
        printf("Going back.\n"); return;
    }
    if (d.specialty[0] == '\0') strcpy(d.specialty, "General");

    d.isAvailable = 1;
    doctors[*count] = d;
    (*count)++;

    saveDoctors(doctors, *count);
    printf("Doctor '%s' added (ID: %d).\n", d.name, d.id);
}

/* ===========================================================
   DELETE DOCTOR
   =========================================================== */
static inline void deleteDoctor(Doctor doctors[], int *count) {
    if (*count == 0) { printf("No doctors to delete.\n"); return; }

    viewDoctors(doctors, *count);

    printf("Enter Doctor ID to delete (0 to go back): ");
    int id;
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }
    if (id == 0) { printf("Going back.\n"); while (getchar() != '\n'); return; }

    /* Find the doctor by ID */
    int found = -1;
    for (int i = 0; i < *count; i++) {
        if (doctors[i].id == id) { found = i; break; }
    }

    if (found == -1) { printf("Doctor ID %d not found.\n", id); return; }

    printf("Delete Dr. %s? (y/n, 0 to go back): ", doctors[found].name);
    char confirm;
    scanf(" %c", &confirm);

    if (confirm == '0') { printf("Going back.\n"); while (getchar() != '\n'); return; }
    if (confirm != 'y' && confirm != 'Y') { printf("Deletion cancelled.\n"); return; }

    /* Shift remaining entries left to fill the gap */
    for (int i = found; i < *count - 1; i++)
        doctors[i] = doctors[i + 1];
    (*count)--;

    saveDoctors(doctors, *count);
    printf("Doctor deleted successfully.\n");
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
