#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DoctorManagement.h"
#include "DoctorFileHandling.h"

/* ===========================================================
   DELETE DOCTOR
   =========================================================== */
static inline void deleteDoctor(Doctor doctors[], int *count) {
    if (*count == 0) { printf("No doctors to delete.\n"); return; }

    viewDoctors(doctors, *count);

    printf("Enter Doctor ID to delete (0 to go back): ");
    int id;
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); return; }
    if (id == 0) { printf("Going back.\n"); while (getchar() != '\n'); return; }

    /* Find the doctor by ID */
    int found = -1;
    for (int i = 0; i < *count; i++) {
        if (doctors[i].id == id) { found = i; break; }
    }

    if (found == -1) { printf("Doctor ID %d not found.\n", id); return; }

    printf("Delete Dr. %s? (y/n, 0 to go back): ", doctors[found].name);
    char confirm;
    scanf(" %c", &confirm);

    if (confirm == '0') { printf("Going back.\n"); while (getchar() != '\n'); return; }
    if (confirm != 'y' && confirm != 'Y') { printf("Deletion cancelled.\n"); return; }

    /* Shift remaining entries left to fill the gap */
    for (int i = found; i < *count - 1; i++)
        doctors[i] = doctors[i + 1];
    (*count)--;

    saveDoctors(doctors, *count);
    printf("Doctor deleted successfully.\n");
}
