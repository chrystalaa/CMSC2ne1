#ifndef VIEWPATIENT_H
#define VIEWPATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // Required for isalpha and isdigit

/* ===========================================================
   VIEW ALL PATIENT RECORDS
   =========================================================== */
static inline void PatientRecords(void) {
    printf("\n==========================================\n");
    printf("         ALL PATIENT RECORDS              \n");
    printf("==========================================\n");

    FILE *fp = fopen("records/patients.txt", "r");
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
#endif /* VIEWPATIENT_H */