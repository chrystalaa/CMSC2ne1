#ifndef ADDPATIENTS_H
#define ADDPATIENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Patient {
    char name[100];
    int age;
    int sex_at_birth;
    char contact[20];
    char address[200];
    float height;
    float weight;
    char medical_history[500];
};

static inline void AddPatients() {
    struct Patient p;

    printf("You are now in the Add Patients section.\n\n");

    while (getchar() != '\n'); // clears newline left by previous scanf in main

    // Name
    printf("Please enter the patient's name: ");
    fgets(p.name, sizeof(p.name), stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    // Age
    printf("Please enter the patient's age: ");
    scanf("%d", &p.age);

    // Sex
    printf("Patient's sex at birth (1. Male, 2. Female): ");
    scanf("%d", &p.sex_at_birth);

    while (getchar() != '\n'); // clears newline left by scanf

    // Contact Number
    printf("Enter contact number: ");
    fgets(p.contact, sizeof(p.contact), stdin);
    p.contact[strcspn(p.contact, "\n")] = '\0';

    // Address
    printf("Enter address: ");
    fgets(p.address, sizeof(p.address), stdin);
    p.address[strcspn(p.address, "\n")] = '\0';

    // Height and Weight
    printf("Enter height (cm): ");
    scanf("%f", &p.height);

    printf("Enter weight (kg): ");
    scanf("%f", &p.weight);

    while (getchar() != '\n'); // clears newline again

    // Medical History
    printf("Please enter the patient's medical history: ");
    fgets(p.medical_history, sizeof(p.medical_history), stdin);
    p.medical_history[strcspn(p.medical_history, "\n")] = '\0';

    // Validation
    if (p.age <= 0 || (p.sex_at_birth != 1 && p.sex_at_birth != 2)) {
        printf("Invalid input. Data not saved.\n");
        return;
    }

    // Save to file
    FILE *fp = fopen("patients.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "Name: %s\nAge: %d\nSex: %s\nContact: %s\n"
                    "Address: %s\nHeight: %.2f cm\nWeight: %.2f kg\n"
                    "Medical History: %s\n\n",
                p.name, p.age, (p.sex_at_birth == 1 ? "Male" : "Female"), 
                p.contact, p.address, p.height, p.weight, p.medical_history);
        fclose(fp);
        printf("\nPatient successfully added and saved.\n");
    } else {
        printf("Error saving patient data.\n");
    }
}

#endif // ADDPATIENTS_H