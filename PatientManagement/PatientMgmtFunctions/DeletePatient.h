#ifndef DELETEPATIENT_H
#define DELETEPATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // Required for isalpha and isdigit
#include <strings.h> // Required for strcasecmp

/* ===========================================================
   DELETE PATIENT  (Rewritten to dynamically parse tags)
   =========================================================== */
static inline void DeletePatient(void) {
    printf("\n==========================================\n");
    printf("          DELETE PATIENT SECTION          \n");
    printf("==========================================\n");

    FILE *fp = fopen("patients.txt", "r");
    if (!fp) { 
        printf("No patient records found.\n"); 
        return; 
    }

    // Clear input buffer leftover from previous menus safely
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    char delete_first[50];
    char delete_last[50];

    // Get First Name
    printf("Enter patient's First Name (0 to go back): ");
    if (!fgets(delete_first, sizeof(delete_first), stdin)) {
        fclose(fp);
        return;
    }
    delete_first[strcspn(delete_first, "\n")] = '\0';

    if (strcmp(delete_first, "0") == 0 || delete_first[0] == '\0') {
        printf("Going back.\n"); 
        fclose(fp); 
        return;
    }

    // Get Last Name
    printf("Enter patient's Last Name (0 to go back): ");
    if (!fgets(delete_last, sizeof(delete_last), stdin)) {
        fclose(fp);
        return;
    }
    delete_last[strcspn(delete_last, "\n")] = '\0';

    if (strcmp(delete_last, "0") == 0 || delete_last[0] == '\0') {
        printf("Going back.\n"); 
        fclose(fp); 
        return;
    }

    /* Write non-matching records to a temp file */
    FILE *tmp = fopen("patients_temp.txt", "w");
    if (!tmp) {
        printf("System error: could not create temp file.\n");
        fclose(fp); 
        return;
    }

    char line[500];
    char extra_line[500];
    int found = 0;

    /* Process the file dynamically by tracking "Name: " indicators */
    while (fgets(line, sizeof(line), fp)) {
        
        // Check if this line introduces a patient record block
        if (strncmp(line, "Name: ", 6) == 0) {
            char file_fname[50] = {0};
            char file_lname[50] = {0};
            
            // Extract the first and last name from the file line string
            sscanf(line + 6, "%s %s", file_fname, file_lname);

            // Check if both first and last names match your deletion target
            if (!found && strcasecmp(file_fname, delete_first) == 0 && strcasecmp(file_lname, delete_last) == 0) {
                found = 1;
                
                // Skip lines belonging to this target until reaching the blank layout separator line
                while (fgets(extra_line, sizeof(extra_line), fp) && strlen(extra_line) > 1) {
                    // Purposefully doing nothing inside loop to skip writing data lines
                }
                continue; // Prevent writing the original block boundary line to temp file
            }
        }

        // If it is a completely different patient, pass the record data cleanly along
        fprintf(tmp, "%s", line);
    }

    fclose(fp);
    fclose(tmp);

    if (found) {
        /* Confirm before making the deletion permanent */
        printf("Are you sure you want to delete patient '%s %s'? (y/n): ", delete_first, delete_last);
        char c;
        scanf(" %c", &c);
        if (c == 'n' || c == 'N') {
            remove("patients_temp.txt");
            printf("Deletion cancelled.\n");
            return;
        }
        remove("patients.txt");
        rename("patients_temp.txt", "patients.txt");
        printf("Patient '%s %s' deleted successfully.\n", delete_first, delete_last);
    } else {
        remove("patients_temp.txt");
        printf("Patient '%s %s' not found.\n", delete_first, delete_last);
    }
}
#endif