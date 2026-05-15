#ifndef SEARCHPATIENT_H
#define SEARCHPATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // Required for isalpha and isdigit
#include <strings.h> // Required for strcasecmp

// Helper function for case-insensitive searching
static inline char* strcasestr_custom(const char* haystack, const char* needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; haystack++) {
        if (toupper((unsigned char)*haystack) == toupper((unsigned char)*needle)) { //make it all caps for comparison
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n; h++, n++) { //needle is for the search term, haystack is for the line we are checking
                if (toupper((unsigned char)*h) != toupper((unsigned char)*n)) break;
            }
            if (!*n) return (char*)haystack;
        }
    }
    return NULL;
}

static inline void SearchPatient(void) {
    printf("\n==========================================\n");
    printf("           SEARCH PATIENT SECTION          \n");
    printf("==========================================\n");

    FILE *fp = fopen("patients.txt", "r");
    if (!fp) { printf("No patient records found.\n"); return; }

    // Use a more robust buffer clear
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    char search_name[100];
    printf("Enter patient name to search (0 to go back): ");
    if (!fgets(search_name, sizeof(search_name), stdin)) { fclose(fp); return; }
    search_name[strcspn(search_name, "\n")] = '\0';

    if (strcmp(search_name, "0") == 0 || search_name[0] == '\0') {
        fclose(fp); return;
    }

    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        // Only look at lines starting with "Name: "
        if (strncmp(line, "Name: ", 6) == 0) {
            // Check if search_name exists within this specific line (ignoring case is harder here, 
            // but at least this prevents matching against addresses/history)
            if (strcasestr_custom(line, search_name)) { 
                found = 1;
                printf("\nPatient Record Found:\n");
                printf("----------------------------------------------------------\n");
                printf("%s", line);
                
                // Print lines until we hit the blank separator line
                while (fgets(line, sizeof(line), fp) && strlen(line) > 1) {
                    printf("%s", line);
                }
                printf("----------------------------------------------------------\n");
                break; 
            }
        }
    }

    if (!found) printf("Patient '%s' not found.\n", search_name);
    fclose(fp);
}
#endif