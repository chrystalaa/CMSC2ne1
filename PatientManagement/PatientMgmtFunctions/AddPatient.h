#ifndef ADDPATIENT_H
#define ADDPATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // Required for isalpha and isdigit

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

typedef struct {
    char fname[50];  // Split name to match your logic
    char lname[50];
    int age;
    int sex_at_birth; 
    int is_senior;
    char contact[20];
    char address[200];
    float height; 
    float weight; 
    char medical_history[500];
} Patient;

// Helper to check if a string is only letters
static inline int is_alpha_only(const char *str) {
    if (strlen(str) == 0) return 0;
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') return 0;
    }
    return 1;
}

// Moved outside of AddPatients
static inline int is_valid_contact(const char *str) {
    if (strlen(str) != 11) return 0;
    for (int i = 0; i < 11; i++) {
        if (!isdigit(str[i])) return 0;
    }
    return 1;
}

/* ===========================================================
   ADD PATIENT
   =========================================================== */
static inline void AddPatients() {

    Patient p; // Corrected typedef usage
    printf("You are now in the Add Patients section.\n\n");

    // Clear buffer once at start
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 

    // --- GIVEN NAME ---
    while (1) {
        printf("Please enter the patient's Given Name (or '0' to go back): ");
        if (!fgets(p.fname, sizeof(p.fname), stdin)) return;
        p.fname[strcspn(p.fname, "\n")] = '\0';

        if (strcmp(p.fname, "0") == 0) return;
        if (is_alpha_only(p.fname)) break;
        printf("Invalid input. Use letters only.\n");
    }

    // --- SURNAME ---
    while (1) {
        printf("Please enter the patient's Surname (or '0' to go back): ");
        if (!fgets(p.lname, sizeof(p.lname), stdin)) return;
        p.lname[strcspn(p.lname, "\n")] = '\0';

        if (strcmp(p.lname, "0") == 0) return;
        if (is_alpha_only(p.lname)) break;
        printf("Invalid input. Use letters only.\n");
    }

    // --- DUPLICATE CHECK ---
    FILE *check_fp = fopen("records/patients.txt", "r");
    if (check_fp != NULL) {
        char line[256], file_fname[50], file_lname[50];
        int found = 0;
        while (fgets(line, sizeof(line), check_fp)) {
            if (strncmp(line, "Name: ", 6) == 0) {
                if (sscanf(line + 6, "%s %s", file_fname, file_lname) == 2) {
                    if (strcasecmp(file_fname, p.fname) == 0 && strcasecmp(file_lname, p.lname) == 0) {
                        found = 1;
                        break;
                    }
                }
            }
        }
        fclose(check_fp);
        if (found) {
            printf("Error: A patient named %s %s already exists.\n", p.fname, p.lname);
            return;
        }
    }

    // --- AGE & SEX ---
    printf("Please enter the patient's age: ");
    if (scanf("%d", &p.age) != 1 || p.age <= 0) {
        printf("Invalid age.\n");
        while (getchar() != '\n');
        return;
    } 
    while (getchar() != '\n'); // Clean buffer

    // Initialize and check senior status
    p.is_senior = 0; 
    if (p.age >= 60) { // Usually 60+ is senior
        printf("Patient is considered a senior citizen.\n");
        p.is_senior = 1;
    }

    while (1) {
    printf("Patient's sex at birth (1. Male, 2. Female): ");
    if (scanf("%d", &p.sex_at_birth) == 1 &&
        (p.sex_at_birth == 1 || p.sex_at_birth == 2)) break;
    printf("Invalid input. Enter 1 or 2.\n");
    while (getchar() != '\n');
    } 
    while (getchar() != '\n'); // Clean buffer

    // --- CONTACT ---
    while (1) {
        printf("Enter contact number (11 digits): ");
        fgets(p.contact, sizeof(p.contact), stdin);
        p.contact[strcspn(p.contact, "\n")] = '\0';
        if (strcmp(p.contact, "0") == 0) return;
        if (is_valid_contact(p.contact)) break;
        printf("Invalid input! Must be 11 digits.\n");
    }

    // --- ADDRESS, HEIGHT, WEIGHT ---
    printf("Enter address: ");
    fgets(p.address, sizeof(p.address), stdin);
    p.address[strcspn(p.address, "\n")] = '\0';

    // --- HEIGHT & WEIGHT ---
    printf("Enter height (cm): ");
    scanf("%f", &p.height);
    printf("Enter weight (kg): ");
    scanf("%f", &p.weight);
    while (getchar() != '\n'); 

    printf("Please enter medical history: ");
    fgets(p.medical_history, sizeof(p.medical_history), stdin);
    p.medical_history[strcspn(p.medical_history, "\n")] = '\0';

    // --- SAVE ---
    FILE *fp = fopen("records/patients.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "Name: %s %s\nAge: %d\nSenior Citizen: %s\nSex: %s\nContact: %s\n"
                    "Address: %s\nHeight: %.2f cm\nWeight: %.2f kg\n"
                    "Medical History: %s\n\n",
                p.fname, p.lname, p.age, 
                (p.is_senior == 1 ? "Yes" : "No"), // Added this!
                (p.sex_at_birth == 1 ? "Male" : "Female"), 
                p.contact, p.address, p.height, p.weight, p.medical_history);
        fclose(fp);
        printf("\nPatient %s %s successfully added.\n", p.fname, p.lname);
    }
    else {
        printf("Error opening file for writing.\n");
    }
}
#endif