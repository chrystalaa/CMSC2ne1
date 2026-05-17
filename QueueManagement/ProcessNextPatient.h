#ifndef PROCESSNEXTPATIENT_H
#define PROCESSNEXTPATIENT_H

/* ===========================================================
   3. PROCESS NEXT PATIENT (Doctor Control Panel)
   =========================================================== */
static inline void processNextPatient(Doctor doctors[], int doctorCount) {
    printf("Enter doctor number (1-%d): ", doctorCount);
    int doc;
    if (scanf("%d", &doc) != 1) { while (getchar() != '\n'); return; }
    doc--;

    if (doc < 0 || doc >= doctorCount) return;

    printf("\nDr. %s's Control Panel\n", doctors[doc].name);
    printf("1. Call Next Priority Patient (Waiting: %d)\n", priorityQueueCount[doc]);
    printf("2. Call Next Regular Patient (Waiting: %d)\n", queueCount[doc]);
    printf("3. Mark Current Patient as Done\n");
    printf("0. Back\nChoice: ");

    int action;
    if (scanf("%d", &action) != 1) { while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    switch (action) {
        case 0: return; // Back
        case 1: // Call Priority
            if (doctorBusy[doc] != 0) { printf("Room is still occupied. Mark current patient as done first.\n"); break; }
            if (priorityQueueCount[doc] == 0) { printf("No priority patients waiting.\n"); break; }
            doctorBusy[doc] = 2;
            printf("Priority Patient %s called into room.\n", priorityQueueSlots[doc][0]);
            break;

        case 2: // Call Regular
            if (doctorBusy[doc] != 0) { printf("Room is still occupied. Mark current patient as done first.\n"); break; }
            if (queueCount[doc] == 0) { printf("No regular patients waiting.\n"); break; }
            doctorBusy[doc] = 1;
            printf("Regular Patient %s called into room.\n", queueSlots[doc][0]);
            break;

        case 3: // Done
            if (doctorBusy[doc] == 0) { printf("No one is in the room.\n"); break; }
            
            if (doctorBusy[doc] == 2) { // Priority patient was inside
                markCompleted(priorityQueueSlots[doc][0]);
                for (int i = 0; i < priorityQueueCount[doc] - 1; i++)
                    strcpy(priorityQueueSlots[doc][i], priorityQueueSlots[doc][i + 1]);
                priorityQueueCount[doc]--;
            } else { // Regular patient was inside
                markCompleted(queueSlots[doc][0]);
                for (int i = 0; i < queueCount[doc] - 1; i++)
                    strcpy(queueSlots[doc][i], queueSlots[doc][i + 1]);
                queueCount[doc]--;
            }
            doctorBusy[doc] = 0;
            printf("Room is now clear.\n");
            break;
        default: printf("Invalid option.\n"); break;
    }
}

#endif /* PROCESSNEXTPATIENT_H */