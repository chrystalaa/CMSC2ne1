#ifndef FUNCTIONHELPERS_H
#define FUNCTIONHELPERS_H

#include <time.h>

// Dependencies are handled by including the root header or ensuring structures exist
// Assumes it has access to struct QAppointment via queuemgmt.h

/* ===========================================================
   HELPER: verifyAndGetAppt
   =========================================================== */
static inline int verifyAndGetAppt(const char *searchID,
                                   struct QAppointment *out,
                                   Doctor doctors[], int doctorCount,
                                   int *docIdx) {
    FILE *fp = fopen("records/appointments.txt", "r");
    if (!fp) return 0;

    char line[360]; 
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\r') continue;

        struct QAppointment tmp;
        if (sscanf(line, "%8[^|]|%49[^|]|%49[^|]|%10[^|]|%19[^|]|%19[^\r\n]",
                   tmp.appointmentID, tmp.patientName, tmp.doctorName, 
                   tmp.date, tmp.type, tmp.status) == 6) {

            if (strcmp(tmp.appointmentID, searchID) == 0 &&
                strcmp(tmp.status, "Scheduled") == 0) {

                *out = tmp;
                *docIdx = -1;

                for (int i = 0; i < doctorCount; i++) {
                    if (strcmp(doctors[i].name, tmp.doctorName) == 0) {
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

    FILE *fp = fopen("records/appointments.txt", "r");
    if (!fp) return;

    char line[360];
    while (fgets(line, sizeof(line), fp) && total < MAX_APPTS_LOCAL) {
        if (sscanf(line, "%8[^|]|%49[^|]|%49[^|]|%10[^|]|%19[^|]|%19[^\r\n]",
                   all[total].appointmentID, all[total].patientName,
                   all[total].doctorName, all[total].date,
                   all[total].type, all[total].status) == 6) {

            if (strcmp(all[total].appointmentID, targetID) == 0)
                strcpy(all[total].status, "Completed");
            total++;
        }
    }
    fclose(fp);

    fp = fopen("records/appointments.txt", "w");
    if (!fp) return;
    for (int i = 0; i < total; i++) {
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                all[i].appointmentID, all[i].patientName, all[i].doctorName, 
                all[i].date, all[i].type, all[i].status);
    }
    fclose(fp);
}

/* ===========================================================
   HELPER: markMissed
   =========================================================== */
static inline void markMissed(void) {
    /* Get today's date in Philippine time */
    time_t now = time(NULL);
    now += 8 * 3600;
    struct tm *t = gmtime(&now);
    char today[11];
    snprintf(today, sizeof(today), "%04d-%02d-%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    struct QAppointment all[MAX_APPTS_LOCAL];
    int total = 0;

    FILE *fp = fopen("records/appointments.txt", "r");
    if (!fp) return;

    char line[360];
    while (fgets(line, sizeof(line), fp) && total < MAX_APPTS_LOCAL) {
        if (sscanf(line, "%8[^|]|%49[^|]|%49[^|]|%10[^|]|%19[^|]|%19[^\r\n]",
                   all[total].appointmentID, all[total].patientName,
                   all[total].doctorName, all[total].date,
                   all[total].type, all[total].status) == 6) {

            /* Mark as Missed if Scheduled and date is today or earlier */
            if (strcmp(all[total].status, "Scheduled") == 0 &&
                strcmp(all[total].date, today) <= 0)
                strcpy(all[total].status, "Missed");

            total++;
        }
    }
    fclose(fp);

    fp = fopen("records/appointments.txt", "w");
    if (!fp) return;
    for (int i = 0; i < total; i++)
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                all[i].appointmentID, all[i].patientName, all[i].doctorName,
                all[i].date, all[i].type, all[i].status);
    fclose(fp);

    printf("[System] End-of-day check complete. Unattended appointments marked as Missed.\n");
}

#endif /* FUNCTIONHELPERS_H */