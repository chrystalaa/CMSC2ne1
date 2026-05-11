#ifndef SEARCHPATIENT_H
#define SEARCHPATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void SearchPatient() {
    printf("You are now in the Search Patient section.\n");

    FILE *fp = fopen("patients.txt", "r");

    if (fp == NULL) {
        printf("Error opening patient records.\n");
        return;
    }

    while (getchar() != '\n'); // clears leftover newline from scanf in main

    char search_name[100];

    printf("Input the name of the patient you want to search: ");
    fgets(search_name, sizeof(search_name), stdin);

    search_name[strcspn(search_name, "\n")] = '\0';

    if (search_name[0] == '\0') {
        printf("No name entered. Please try again.\n");
        fclose(fp);
        return;
    }

    char line[500];
    int found = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {

        // Check if the line contains the patient's name
        if (strstr(line, search_name) != NULL) {

            found = 1;

            printf("\nPatient Record Found:\n\n");

            // Print current line
            printf("%s", line);

            // Print remaining patient information
            for (int i = 0; i < 8; i++) {
                fgets(line, sizeof(line), fp);
                printf("%s", line);
            }

            break;
        }
    }

    if (!found) {
        printf("Patient not found.\n");
    }

    fclose(fp);
}

#endif // SEARCHPATIENT_H