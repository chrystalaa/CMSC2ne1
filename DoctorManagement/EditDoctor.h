#ifndef EDITDOCTOR_H
#define EDITDOCTOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "DoctorFileHandling.h"
#include "ViewDoctor.h"

/* ===========================================================
    EDIT DOCTOR  
   =========================================================== */
static inline void editDoctor(Doctor doctors[], int doctorCount) {
    while (getchar() != '\n');

    printf("\n--- Edit Doctor Details (enter 0 at any prompt to go back) ---\n");

    /*Reads the numeric ID into text buffer using fgets */
    char idBuffer[20];
    printf("Enter Doctor ID to edit:  ");
    fgets(idBuffer, sizeof(idBuffer), stdin);
    idBuffer[strcspn(idBuffer, "\n")] = '\0';
    /*Checks if user wants to back out early*/
    if (strcmp(idBuffer, "0") == 0 || idBuffer[0] == '\0') { printf("Going back.\n"); return; }

    /*String to int conversion, turns text buffers into actal integer values*/
    int searchId = atoi(idBuffer);
    /* Find Doctor by ID */
    int idx = -1;
    for (int i = 0; i <doctorCount; i++) {
        if (doctors[i].id == searchId){ idx = i; break; }
    }

    /*If idx == -1, no doctor has that ID*/
    if (idx == -1) { printf("Doctor ID '%d' not found.\n", searchId); return; }

   /*Displaying current details*/
    printf("\nCurrent details:\n");
    viewDoctors(&doctors[idx], 1);

    /*Menu*/
    printf("\nWhat to edit?\n");
    printf("1. Doctor name\n");
    printf("2. Specialty\n");
    printf("0. Go back\n");
    printf("Choice: ");

    int opt;
    if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    switch (opt) {
        case 0: printf("Going back.\n"); return;

        case 1: {   /* Edit Doctors name */
        char newName[50];

            printf("New Doctor name (0 to go back): ");
            fgets(newName, sizeof(newName), stdin);
            newName[strcspn(newName, "\n")] = '\0';
            /*If user types 0 or blank, goes back without changing anything*/
            if (strcmp(newName, "0") == 0 ||
            newName[0] == '\0') {
                printf("Going back.\n"); return;
            }
            /*Copies string to prevent buffer overflow*/
            strncpy(doctors[idx].name, newName, 49);
            doctors[idx].name[49] = '\0';
            break;
        }

        case 2: {  /* Edit Specialty */
            printf("New Specialty, (0 to go back): ");
            fgets(doctors[idx].specialty, sizeof(doctors[idx].specialty), stdin);
            doctors[idx].specialty[strcspn(doctors[idx].specialty, "\n")] = '\0';
            if (strcmp(doctors[idx].specialty, "0") == 0 || doctors[idx].specialty[0] == '\0') {
                printf("Going back.\n"); return;
            }
            break;
        }
            default:
            printf("Invalid option. \n"); return;
    }
    /*Saves file and prints updated doctor list.*/
    saveDoctors(doctors, doctorCount);
    printf("\nDoctor details updated successfully!\n");
    viewDoctors(&doctors[idx], 1);
}

#endif /* EDITDOCTOR_H */