#ifndef VIEWDOCTOR_H
#define VIEWDOCTOR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* ===========================================================
   VIEW DOCTORS
   =========================================================== */
static inline void viewDoctors(Doctor doctors[], int count) {
    printf("\n==========================================\n");
    printf("              DOCTOR LIST                 \n");
    printf("==========================================\n");

    if (count == 0) {
        printf("  No doctors on record.\n");
    } else {
        printf("%-4s %-22s %-20s %-11s\n",
               "ID", "Name", "Specialty", "Status");
        printf("----------------------------------------------------------\n");
        for (int i = 0; i < count; i++)
            printf("%-4d %-22s %-20s %-11s\n",
                   doctors[i].id,
                   doctors[i].name,
                   doctors[i].specialty,
                   doctors[i].isAvailable ? "Available" : "Unavailable");
    }
    printf("==========================================\n");
}


#endif // VIEWDOCTOR_H