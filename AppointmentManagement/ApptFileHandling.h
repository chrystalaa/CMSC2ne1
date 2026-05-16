#ifndef APPTFILEHANDLING_H
#define APPTFILEHANDLING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ApptStructure.h"

#define MAX_APPOINTMENTS 100

/* ===========================================================
   FILE I/O
   =========================================================== */

/* Overwrite appointments.txt with the current in-memory array */
static inline void saveAppointments(Appointment *appts, int total) {
    FILE *fp = fopen("appointments.txt", "w");
    if (!fp) { printf("Error saving appointments.\n"); return; }

    for (int i = 0; i < total; i++)
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                appts[i].appointmentID, appts[i].patientName,
                appts[i].doctorName,    appts[i].date,
                appts[i].type,          appts[i].status);

    fclose(fp);
}

/* Load appointments from file; returns count. Opens with "a+" to create if missing */
static inline int loadAppointments(Appointment *appts) {
    FILE *fp = fopen("appointments.txt", "a+");  /* create if not exists */
    if (!fp) return 0;
    rewind(fp);

    int  total = 0;
    char line[300];

    while (fgets(line, sizeof(line), fp) && total < MAX_APPOINTMENTS) {
        if (line[0] == '\n' || line[0] == '\r'|| line[0] == '\0') 
            continue;  /* skip blank lines */
        int result = sscanf(line, "%7[^|]|%49[^|]|%49[^|]|%10[^|]|%19[^|]|%14[^\n]",
                            appts[total].appointmentID,     appts[total].patientName,
                            appts[total].doctorName,        appts[total].date,
                            appts[total].type,              appts[total].status);
        if (result == 6)
            total++;
    }

    fclose(fp);
    return total;
}

#endif /* APPTFILEHANDLING_H */