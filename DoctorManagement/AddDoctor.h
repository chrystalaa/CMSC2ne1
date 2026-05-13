#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DoctorManagement.h"
#include "DoctorFileHandling.h"

/* ===========================================================
   ADD DOCTOR
   =========================================================== */
static inline void addDoctor(Doctor doctors[], int *count) {
    if (*count >= MAX_DOCTORS) {
        printf("Maximum number of doctors (%d) reached.\n", MAX_DOCTORS);
        return;
    }

    while (getchar() != '\n');   /* flush */

    Doctor d;
    /* Auto-increment ID from last entry */
    d.id = (*count > 0) ? doctors[*count - 1].id + 1 : 1;

    printf("Enter doctor name (0 to go back): ");
    fgets(d.name, sizeof(d.name), stdin);
    d.name[strcspn(d.name, "\n")] = '\0';
    if (strcmp(d.name, "0") == 0 || d.name[0] == '\0') {
        printf("Going back.\n"); return;
    }

    printf("Enter specialty (0 to go back): ");
    fgets(d.specialty, sizeof(d.specialty), stdin);
    d.specialty[strcspn(d.specialty, "\n")] = '\0';
    if (strcmp(d.specialty, "0") == 0) {
        printf("Going back.\n"); return;
    }
    if (d.specialty[0] == '\0') strcpy(d.specialty, "General");

    d.isAvailable = 1;
    doctors[*count] = d;
    (*count)++;

    saveDoctors(doctors, *count);
    printf("Doctor '%s' added (ID: %d).\n", d.name, d.id);
}
