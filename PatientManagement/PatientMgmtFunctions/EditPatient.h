#ifndef EDITPATIENT_H
#define EDITPATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AddPatient.h"  // Reuse Patient struct and helpers

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

#define MAX_PATIENTS 100

/* Parse a single patient block from file into a Patient struct */
static inline int parse_patient(FILE *fp, Patient *p) {
    char line[512];
    int fields_read = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (line[0] == '\0' || line[0] == '\r'){
            // Empty line = end of record
            return (fields_read > 0) ? 1 : 0;
        }
        
        if (strncmp(line, "Name: ", 6) == 0) {
            sscanf(line + 6, "%49s %49s", p->fname, p->lname);
            fields_read++;
        } else if (strncmp(line, "Age: ", 5) == 0) {
            p->age = atoi(line + 5);
            fields_read++;
        } else if (strncmp(line, "Senior Citizen: ", 16) == 0) {
            p->is_senior = (strcmp(line + 16, "Yes") == 0) ? 1 : 0;
            fields_read++;
        } else if (strncmp(line, "Sex: ", 5) == 0) {
            p->sex_at_birth = (strcmp(line + 5, "Male") == 0) ? 1 : 2;
            fields_read++;
        } else if (strncmp(line, "Contact: ", 9) == 0) {
            strncpy(p->contact, line + 9, sizeof(p->contact) - 1);
            fields_read++;
        } else if (strncmp(line, "Address: ", 9) == 0) {
            strncpy(p->address, line + 9, sizeof(p->address) - 1);
            fields_read++;
        } else if (strncmp(line, "Height: ", 8) == 0) {
            p->height = atof(line + 8);
            fields_read++;
        } else if (strncmp(line, "Weight: ", 8) == 0) {
            p->weight = atof(line + 8);
            fields_read++;
        } else if (strncmp(line, "Medical History: ", 17) == 0) {
            strncpy(p->medical_history, line + 17, sizeof(p->medical_history) - 1);
            fields_read++;
        }
    }
    return (fields_read > 0) ? 1 : 0;
}

/* Write all patients back to file */
static inline void save_all_patients(Patient patients[], int count) {
    FILE *fp = fopen("records/patients.txt", "w");
    if (!fp) {
        printf("Error: Could not open file for writing.\n");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(fp, "Name: %s %s\nAge: %d\nSenior Citizen: %s\nSex: %s\nContact: %s\n"
                    "Address: %s\nHeight: %.2f cm\nWeight: %.2f kg\n"
                    "Medical History: %s\n\n",
                patients[i].fname, patients[i].lname, patients[i].age,
                (patients[i].is_senior == 1 ? "Yes" : "No"),
                (patients[i].sex_at_birth == 1 ? "Male" : "Female"),
                patients[i].contact, patients[i].address,
                patients[i].height, patients[i].weight,
                patients[i].medical_history);
    }
    fclose(fp);
}

/* Display patient info */
static inline void display_patient(Patient *p) {
    printf("\n--- Current Patient Info ---\n");
    printf("1. Name: %s %s\n", p->fname, p->lname);
    printf("2. Age: %d\n", p->age);
    printf("3. Senior Citizen: %s\n", p->is_senior ? "Yes" : "No");
    printf("4. Sex: %s\n", p->sex_at_birth == 1 ? "Male" : "Female");
    printf("5. Contact: %s\n", p->contact);
    printf("6. Address: %s\n", p->address);
    printf("7. Height: %.2f cm\n", p->height);
    printf("8. Weight: %.2f kg\n", p->weight);
    printf("9. Medical History: %s\n", p->medical_history);
    printf("-----------------------------\n");
}

/* ===========================================================
   EDIT PATIENT
   =========================================================== */
static inline void EditPatient() {
    Patient patients[MAX_PATIENTS];
    int patient_count = 0;
    
    printf("You are now in the Edit Patient section.\n\n");
    
    // Load all patients from file
    FILE *fp = fopen("records/patients.txt", "r");
    if (!fp) {
        printf("No patient records found.\n");
        return;
    }
    
    while (patient_count < MAX_PATIENTS && parse_patient(fp, &patients[patient_count])) {
        patient_count++;
    }
    fclose(fp);
    
    if (patient_count == 0) {
        printf("No patient records found.\n");
        return;
    }
    
    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // Get search name
    char search_fname[50], search_lname[50];
    
    printf("Enter patient's Given Name to search (or '0' to go back): ");
    if (!fgets(search_fname, sizeof(search_fname), stdin)) return;
    search_fname[strcspn(search_fname, "\n")] = '\0';
    if (strcmp(search_fname, "0") == 0) return;
    
    printf("Enter patient's Surname: ");
    if (!fgets(search_lname, sizeof(search_lname), stdin)) return;
    search_lname[strcspn(search_lname, "\n")] = '\0';
    
    // Find patient
    int found_index = -1;
    for (int i = 0; i < patient_count; i++) {
        if (strcasecmp(patients[i].fname, search_fname) == 0 &&
            strcasecmp(patients[i].lname, search_lname) == 0) {
            found_index = i;
            break;
        }
    }
    
    if (found_index == -1) {
        printf("Patient '%s %s' not found.\n", search_fname, search_lname);
        return;
    }
    
    Patient *p = &patients[found_index];
    
    // Edit loop
    int editing = 1;
    while (editing) {
        display_patient(p);
        
        printf("\nSelect field to edit (1-9), or 0 to save and exit: ");
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');
        
        char buffer[500];
        
        switch (choice) {
            case 0:
                editing = 0;
                break;
                
            case 1: // Name
                printf("Enter new Given Name: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (is_alpha_only(buffer)) {
                    strncpy(p->fname, buffer, sizeof(p->fname) - 1);
                } else {
                    printf("Invalid name.\n");
                }
                
                printf("Enter new Surname: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (is_alpha_only(buffer)) {
                    strncpy(p->lname, buffer, sizeof(p->lname) - 1);
                } else {
                    printf("Invalid name.\n");
                }
                break;
                
            case 2: // Age
                printf("Enter new age: ");
                if (scanf("%d", &p->age) == 1 && p->age > 0) {
                    p->is_senior = (p->age >= 60) ? 1 : 0;
                    printf("Senior status updated automatically.\n");
                } else {
                    printf("Invalid age.\n");
                }
                while (getchar() != '\n');
                break;
                
            case 3: // Senior (auto-calculated, but allow override)
                while (1) {
                    printf("Is patient a senior citizen? (1=Yes, 0=No): ");
                    if (scanf("%d", &p->is_senior) == 1 &&
                        (p->is_senior == 0 || p->is_senior == 1)) break;
                    printf("Invalid input. Enter 0 or 1.\n");
                    while (getchar() != '\n');
                }
                while (getchar() != '\n');
                break;
                
            case 4: // Sex
                while (1) {
                    printf("Enter sex at birth (1=Male, 2=Female): ");
                    if (scanf("%d", &p->sex_at_birth) == 1 &&
                        (p->sex_at_birth == 1 || p->sex_at_birth == 2)) break;
                    printf("Invalid input. Enter 1 or 2.\n");
                    while (getchar() != '\n');
                }
                while (getchar() != '\n');
                break;
                
            case 5: // Contact
                printf("Enter new contact (11 digits): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (is_valid_contact(buffer)) {
                    strncpy(p->contact, buffer, sizeof(p->contact) - 1);
                } else {
                    printf("Invalid contact number.\n");
                }
                break;
                
            case 6: // Address
                printf("Enter new address: ");
                fgets(p->address, sizeof(p->address), stdin);
                p->address[strcspn(p->address, "\n")] = '\0';
                break;
                
            case 7: // Height
                while (1) {
                    printf("Enter new height (cm): ");
                    if (scanf("%f", &p->height) == 1 && p->height > 0) break;
                    printf("Invalid height.\n");
                    while (getchar() != '\n');
                }
                while (getchar() != '\n');
                break;
                
            case 8: // Weight
                while (1) {
                    printf("Enter new weight (kg): ");
                    if (scanf("%f", &p->weight) == 1 && p->weight > 0) break;
                    printf("Invalid weight.\n");
                    while (getchar() != '\n');
                }
                while (getchar() != '\n');
                break;
                
            case 9: // Medical History
                printf("Enter new medical history: ");
                fgets(p->medical_history, sizeof(p->medical_history), stdin);
                p->medical_history[strcspn(p->medical_history, "\n")] = '\0';
                break;
                
            default:
                printf("Invalid choice.\n");
        }
    }
    
    // Save changes
    save_all_patients(patients, patient_count);
    printf("\nPatient record updated successfully.\n");
}

#endif /* EDITPATIENT_H */
