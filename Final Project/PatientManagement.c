//AddPatients
//SearchPatient
//PatientRecords
//DeletePatient
#include "AddPatients.h"
#include "SearchPatient.h"
#include "PatientRecords.h"
#include "DeletePatient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void AddPatients();
void SearchPatient();
void PatientRecords();
void DeletePatient();

int main(){

    while (1) {
        int option;

        printf("\nWelcome to the Patient Management System\n");
        printf("1. Add Patients\n2. Search Patient\n3. Patient Records\n4. Delete Patient\n5. Exit\n");
        printf("Select option: ");
        
        if (scanf("%d", &option) != 1) break;
        if (option == 5) break;

        switch (option) {
            case 1: AddPatients(); break;
            case 2: SearchPatient(); break;
            case 3: PatientRecords(); break;
            case 4: DeletePatient(); break;
            default: printf("Invalid option.\n");
        }
    }
    return 0;
}