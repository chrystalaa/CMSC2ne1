#ifndef QUEUEMGMT_CHECKIN_H
#define QUEUEMGMT_CHECKIN_H

/* ===========================================================
   1. CHECK-IN PATIENT
   =========================================================== */
static inline void checkInPatient(Doctor doctors[], int doctorCount) {
    while (getchar() != '\n'); 

    printf("\n--- Clinic Check-in ---\n");
    char inputID[20];
    printf("Enter Appointment ID: ");
    fgets(inputID, sizeof(inputID), stdin);
    inputID[strcspn(inputID, "\n")] = '\0';

    struct QAppointment appt;
    int docIdx = -1;

    if (!verifyAndGetAppt(inputID, &appt, doctors, doctorCount, &docIdx)) {
        printf("Invalid ID or Appointment not eligible for check-in.\n");
        return;
    }

    /* MULTI-CONDITION ROUTING */
    if (strcmp(appt.type, "PWD") == 0 || 
        strcmp(appt.type, "Pregnant") == 0 || 
        strcmp(appt.type, "Senior Citizen") == 0) {
        
        if (priorityQueueCount[docIdx] >= MAX_QUEUE) {
            printf("Priority queue is full.\n"); return;
        }
        strcpy(priorityQueueSlots[docIdx][priorityQueueCount[docIdx]], inputID);
        priorityQueueCount[docIdx]++;
        printf("Success! [Priority Queue: %s] Position: #%d\n", appt.type, priorityQueueCount[docIdx]);
    } 
    else {
        if (queueCount[docIdx] >= MAX_QUEUE) {
            printf("Regular queue is full.\n"); return;
        }
        strcpy(queueSlots[docIdx][queueCount[docIdx]], inputID);
        queueCount[docIdx]++;
        printf("Success! [Regular Queue] Position: #%d\n", queueCount[docIdx]);
    }
    
    printf("Patient: %s | Dr. %s\n", appt.patientName, doctors[docIdx].name);
}

#endif /* QUEUEMGMT_CHECKIN_H */