#ifndef QUEUEMGMT_STATUS_H
#define QUEUEMGMT_STATUS_H

/* ===========================================================
   2. VIEW QUEUE STATUS
   =========================================================== */
static inline void viewQueueStatus(Doctor doctors[], int doctorCount) {
    printf("\n============================================================\n");
    printf("                LIVE CLINIC QUEUE MONITOR                   \n");
    printf("============================================================\n");

    for (int i = 0; i < doctorCount; i++) {
        printf("Dr. %-15s | ", doctors[i].name);

        // Display Priority Queue
        printf("PRIORITY: ");
        if (priorityQueueCount[i] == 0) printf("[-] ");
        else {
            for (int j = 0; j < priorityQueueCount[i]; j++)
                printf("[%s%s] ", priorityQueueSlots[i][j], (j==0 && doctorBusy[i]==2) ? "*" : "");
        }

        // Display Regular Queue
        printf("| REGULAR: ");
        if (queueCount[i] == 0) printf("[-] ");
        else {
            for (int j = 0; j < queueCount[i]; j++)
                printf("[%s%s] ", queueSlots[i][j], (j==0 && doctorBusy[i]==1) ? "*" : "");
        }
        printf("\n");
    }
    printf("(* indicates patient currently in room)\n");
    printf("============================================================\n");
}

#endif /* QUEUEMGMT_STATUS_H */