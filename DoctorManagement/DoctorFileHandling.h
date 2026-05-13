#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DoctorManagement.h"

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
