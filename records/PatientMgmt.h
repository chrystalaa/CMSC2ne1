/*
 * PatientMgmt.h
 * ============================================================
 * Handles patient records stored in patients.txt:
 *   - Add, Search, View all, Delete
 *
 * patients.txt format (human-readable, 9 lines per record):
 *   Name: ...
 *   Age: ...
 *   Sex: Male|Female
 *   Contact: ...
 *   Address: ...
 *   Height: ... cm
 *   Weight: ... kg
 *   Medical History: ...
 *   <blank line>
 *
 * Concepts used: structs, file handling, strings, functions,
 *                pointers  (CMSC21 Units 1-9)
 * ============================================================
 */

#ifndef PATIENTMGMT_H
#define PATIENTMGMT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Patient struct (Unit 8) ---- */
struct Patient {
    char  name[100];
    int   age;
    int   sex_at_birth;        /* 1 = Male, 2 = Female */
    char  contact[20];
    char  address[200];
    float height;              /* centimetres */
    float weight;              /* kilograms   */
    char  medical_history[500];
};

/* ---- Helper: read a line with fgets; return 1 if user typed "0" ---- */
static inline int readLine(char *buf, int sz, const char *prompt) {
    printf("%s", prompt);
    if (!fgets(buf, sz, stdin)) return 0;
    buf[strcspn(buf, "\n")] = '\0';
    return (strcmp(buf, "0") != 0);   /* 0 means "go back" → return 0 */
}

/* ===========================================================
   ADD PATIENT  (Unit 9: File Handling)
   =========================================================== */
static inline void AddPatients(void) {
    struct Patient p;

    printf("\n==========================================\n");
    printf("           ADD PATIENT SECTION            \n");
    printf("  (enter 0 at any prompt to go back)      \n");
    printf("==========================================\n");

    while (getchar() != '\n');   /* flush leftover newline */

    /* Name */
    if (!readLine(p.name, sizeof(p.name), "Patient name: ")
            || p.name[0] == '\0') {
        printf("Going back.\n"); return;
    }

    /* Age — read as string first, then convert */
    char buf[20];
    if (!readLine(buf, sizeof(buf), "Age: ")) {
        printf("Going back.\n"); return;
    }
    p.age = atoi(buf);
    if (p.age <= 0) { printf("Invalid age.\n"); return; }

    /* Sex */
    if (!readLine(buf, sizeof(buf), "Sex at birth (1=Male, 2=Female): ")) {
        printf("Going back.\n"); return;
    }
    p.sex_at_birth = atoi(buf);
    if (p.sex_at_birth != 1 && p.sex_at_birth != 2) {
        printf("Invalid sex input.\n"); return;
    }

    /* Contact */
    if (!readLine(p.contact, sizeof(p.contact), "Contact number: ")) {
        printf("Going back.\n"); return;
    }

    /* Address */
    if (!readLine(p.address, sizeof(p.address), "Address: ")) {
        printf("Going back.\n"); return;
    }

    /* Height */
    if (!readLine(buf, sizeof(buf), "Height in cm: ")) {
        printf("Going back.\n"); return;
    }
    p.height = (float)atof(buf);

    /* Weight */
    if (!readLine(buf, sizeof(buf), "Weight in kg: ")) {
        printf("Going back.\n"); return;
    }
    p.weight = (float)atof(buf);

    /* Medical history */
    if (!readLine(p.medical_history, sizeof(p.medical_history),
                  "Medical history (or 'None'): ")) {
        printf("Going back.\n"); return;
    }
    if (p.medical_history[0] == '\0') strcpy(p.medical_history, "None");

    /* Append record to patients.txt */
    FILE *fp = fopen("patients.txt", "a");
    if (!fp) { printf("Error saving patient data.\n"); return; }

    fprintf(fp,
        "Name: %s\nAge: %d\nSex: %s\nContact: %s\n"
        "Address: %s\nHeight: %.2f cm\nWeight: %.2f kg\n"
        "Medical History: %s\n\n",
        p.name, p.age,
        (p.sex_at_birth == 1 ? "Male" : "Female"),
        p.contact, p.address, p.height, p.weight, p.medical_history);

    fclose(fp);
    printf("\nPatient '%s' added successfully.\n", p.name);
}

/* ===========================================================
   SEARCH PATIENT  (Unit 7: Strings, Unit 9: File Handling)
   =========================================================== */
static inline void SearchPatient(void) {
    printf("\n==========================================\n");
    printf("          SEARCH PATIENT SECTION          \n");
    printf("==========================================\n");

    FILE *fp = fopen("patients.txt", "r");
    if (!fp) { printf("No patient records found.\n"); return; }

    while (getchar() != '\n');

    char search_name[100];
    printf("Enter patient name to search (0 to go back): ");
    fgets(search_name, sizeof(search_name), stdin);
    search_name[strcspn(search_name, "\n")] = '\0';

    if (strcmp(search_name, "0") == 0 || search_name[0] == '\0') {
        printf("Going back.\n"); fclose(fp); return;
    }

    char line[500];
    int  found = 0;

    /* Scan line-by-line for the name */
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, search_name)) {
            found = 1;
            printf("\nPatient Record Found:\n");
            printf("----------------------------------------------------------\n");
            printf("%s", line);
            /* Print the next 8 lines (the remaining fields) */
            for (int i = 0; i < 8; i++) {
                if (fgets(line, sizeof(line), fp))
                    printf("%s", line);
            }
            printf("----------------------------------------------------------\n");
            break;
        }
    }

    if (!found) printf("Patient '%s' not found.\n", search_name);
    fclose(fp);
}

/* ===========================================================
   VIEW ALL PATIENT RECORDS
   =========================================================== */
static inline void PatientRecords(void) {
    printf("\n==========================================\n");
    printf("         ALL PATIENT RECORDS              \n");
    printf("==========================================\n");

    FILE *fp = fopen("patients.txt", "r");
    if (!fp) {
        printf("No patient database found. Add a patient first.\n");
        printf("==========================================\n");
        return;
    }

    char line[500];
    int  lineCount = 0;

    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
        lineCount++;
    }

    if (lineCount == 0) printf("Patient database is empty.\n");

    printf("\n==========================================\n");
    printf("           END OF RECORDS                 \n");
    printf("==========================================\n");
    fclose(fp);
}

/* ===========================================================
   DELETE PATIENT  (rewrite file without the target record)
   =========================================================== */
static inline void DeletePatient(void) {
    printf("\n==========================================\n");
    printf("          DELETE PATIENT SECTION          \n");
    printf("==========================================\n");

    FILE *fp = fopen("patients.txt", "r");
    if (!fp) { printf("No patient records found.\n"); return; }

    while (getchar() != '\n');

    char delete_name[100];
    printf("Enter full name of patient to delete (0 to go back): ");
    fgets(delete_name, sizeof(delete_name), stdin);
    delete_name[strcspn(delete_name, "\n")] = '\0';

    if (strcmp(delete_name, "0") == 0 || delete_name[0] == '\0') {
        printf("Going back.\n"); fclose(fp); return;
    }

    /* Write non-matching records to a temp file */
    FILE *tmp = fopen("patients_temp.txt", "w");
    if (!tmp) {
        printf("System error: could not create temp file.\n");
        fclose(fp); return;
    }

    char line[500];
    int  found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (!found && strstr(line, delete_name)) {
            found = 1;
            /* Skip the remaining 8 field lines + blank separator */
            for (int i = 0; i < 9; i++) fgets(line, sizeof(line), fp);
            continue;   /* don't copy this record */
        }
        fprintf(tmp, "%s", line);
    }

    fclose(fp);
    fclose(tmp);

    if (found) {
        /* Confirm before making the deletion permanent */
        printf("Delete patient '%s'? (y/n): ", delete_name);
        char c;
        scanf(" %c", &c);
        if (c == 'n' || c == 'N') {
            remove("patients_temp.txt");
            printf("Deletion cancelled.\n");
            return;
        }
        remove("patients.txt");
        rename("patients_temp.txt", "patients.txt");
        printf("Patient '%s' deleted successfully.\n", delete_name);
    } else {
        remove("patients_temp.txt");
        printf("Patient '%s' not found.\n", delete_name);
    }
}

/* ===========================================================
   PATIENT MANAGEMENT MENU
   =========================================================== */
static inline void PatientManagement(void) {
    int opt;
    while (1) {
        printf("\n==========================================\n");
        printf("        PATIENT MANAGEMENT MENU          \n");
        printf("==========================================\n");
        printf("1. Add Patient\n");
        printf("2. Search Patient\n");
        printf("3. View Patient Records\n");
        printf("4. Delete Patient\n");
        printf("0. Back to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); continue; }

        switch (opt) {
            case 1: AddPatients();    break;
            case 2: SearchPatient();  break;
            case 3: PatientRecords(); break;
            case 4: DeletePatient();  break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

#endif /* PATIENTMGMT_H */
