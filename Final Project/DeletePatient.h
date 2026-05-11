#ifndef DELETEPATIENT_H
#define DELETEPATIENT_H

#include <stdio.h>  
#include <stdlib.h>
#include <string.h>

static inline void DeletePatient() {
    printf("\n--- Delete Patient Section ---\n");

    FILE *fp = fopen("patients.txt", "r");
    if (fp == NULL) {
        printf("Error: No records found to delete.\n");
        return;
    }

    // Clear buffer from main menu scanf
    while (getchar() != '\n'); 

    char delete_name[100];
    printf("Enter the full name of the patient to delete: ");
    fgets(delete_name, sizeof(delete_name), stdin);
    delete_name[strcspn(delete_name, "\n")] = '\0';

    if (delete_name[0] == '\0') {
        printf("Deletion cancelled.\n");
        fclose(fp);
        return;
    }

    FILE *temp_fp = fopen("temp.txt", "w");
    if (temp_fp == NULL) {
        printf("System Error: Could not create temporary file.\n");
        fclose(fp);
        return;
    }

    char line[500];
    int found = 0;

    // Logic: Copy everything EXCEPT the target patient
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strstr(line, delete_name) != NULL) {
            found = 1;
            // Skip the next 8 lines associated with this patient
            for (int i = 0; i < 8; i++) {
                fgets(line, sizeof(line), fp);
            }
            // Skip the blank line usually at the end of a record
            fgets(line, sizeof(line), fp); 
            continue; 
        }
        fprintf(temp_fp, "%s", line);
    }

    fclose(fp);
    fclose(temp_fp);

    if (found) {
        remove("patients.txt");
        rename("temp.txt", "patients.txt");
        printf("Successfully deleted record for: %s\n", delete_name);
    } else {
        remove("temp.txt"); // Clean up the temp file if nothing was deleted
        printf("Patient '%s' not found.\n", delete_name);
    }
}

#endif // DELETEPATIENT_H