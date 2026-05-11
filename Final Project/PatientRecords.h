#ifndef PATIENTRECORDS_H
#define PATIENTRECORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void PatientRecords() {
    printf("\n==========================================\n");
    printf("        ALL PATIENT RECORDS               \n");
    printf("==========================================\n");

    FILE *fp = fopen("patients.txt", "r");

    if (fp == NULL) {
        printf("Error: No database found. Please add a patient first.\n");
        printf("==========================================\n");
        return;
    }

    char line[500];
    int line_count = 0;

    // Read and print every line until the end of the file
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
        line_count++;
    }

    if (line_count == 0) {
        printf("The patient database is currently empty.\n");
    }

    printf("\n==========================================\n");
    printf("          END OF RECORDS                  \n");
    printf("==========================================\n");

    fclose(fp);
}

#endif // PATIENTRECORDS_H