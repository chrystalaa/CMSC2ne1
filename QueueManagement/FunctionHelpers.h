#ifndef QUEUEMGMT_HELPERS_H
#define QUEUEMGMT_HELPERS_H

// Dependencies are handled by including the root header or ensuring structures exist
// Assumes it has access to struct QAppointment via queuemgmt.h

/* ===========================================================
   HELPER: verifyAndGetAppt
   =========================================================== */
static inline int verifyAndGetAppt(const char *searchID,
                                   struct QAppointment *out,
                                   Doctor doctors[], int doctorCount,
                                   int *docIdx) {
    FILE *fp = fopen("appointments.txt", "r");
    if (!fp) return 0;

    char line[360]; 
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\r') continue;

        struct QAppointment tmp;
        if (sscanf(line, "%19[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]",
                   tmp.appointmentID, tmp.patientName, tmp.doctorName, 
                   tmp.date, tmp.status, tmp.type) == 6) {

            if (strcmp(tmp.appointmentID, searchID) == 0 &&
                strcmp(tmp.status, "Scheduled") == 0) {

                *out = tmp;
                *docIdx = -1;

                for (int i = 0; i < doctorCount; i++) {
                    if (strstr(doctors[i].name, tmp.doctorName) ||
                        strstr(tmp.doctorName, doctors[i].name)) {
                        *docIdx = i; break;
                    }
                }
                found = 1;
                break;
            }
        }
    }
    fclose(fp);
    return found;
}

/* ===========================================================
   HELPER: markCompleted
   =========================================================== */
static inline void markCompleted(const char *targetID) {
    struct QAppointment all[MAX_APPTS_LOCAL];
    int total = 0;

    FILE *fp = fopen("appointments.txt", "r");
    if (!fp) return;

    char line[360];
    while (fgets(line, sizeof(line), fp) && total < MAX_APPTS_LOCAL) {
        if (sscanf(line, "%19[^|]|%49[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]",
                   all[total].appointmentID, all[total].patientName,
                   all[total].doctorName, all[total].date,
                   all[total].status, all[total].type) == 6) {

            if (strcmp(all[total].appointmentID, targetID) == 0)
                strcpy(all[total].status, "Completed");
            total++;
        }
    }
    fclose(fp);

    fp = fopen("appointments.txt", "w");
    if (!fp) return;
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                all[i].appointmentID, all[i].patientName, all[i].doctorName, 
                all[i].date, all[i].status, all[i].type);
    }
    fclose(fp);
}

#endif /* QUEUEMGMT_HELPERS_H */