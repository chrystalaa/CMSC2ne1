#ifndef PATIENTMGMT_H
#define PATIENTMGMT_H

#include <stdio.h>
#include "AddPatient.h"
#include "SearchPatient.h"
#include "PatientRecords.h"
#include "DeletePatient.h"
#include "EditPatient.h"

/* ===========================================================
   PATIENT MANAGEMENT MENU
   =========================================================== */
static inline void PatientManagement(void) {
    int opt;
    while (1) {
        printf("\n==========================================\n");
        printf("        PATIENT MANAGEMENT MENU          \n");
        printf("==========================================\n");
        printf("1. Add Patient\n");
        printf("2. Search Patient\n");
        printf("3. View Patient Records\n");
        printf("4. Delete Patient\n");
        printf("5. Edit Patient\n");
        printf("0. Back to Main Menu\n");
        printf("Select option: ");

        if (scanf("%d", &opt) != 1) { while (getchar() != '\n'); continue; }

        switch (opt) {
            case 1: AddPatients();    break;
            case 2: SearchPatient();  break;
            case 3: PatientRecords(); break;
            case 4: DeletePatient();  break;
            case 5: EditPatient();    break;
            case 0: return;
            default: printf("Invalid option.\n");
        }
    }
}

#endif /* PATIENTMGMT_H */
