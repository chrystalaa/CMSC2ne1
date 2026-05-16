#ifndef APPTSTRUCTURE_H
#define APPTSTRUCTURE_H

#define MAX_APPOINTMENTS 100

/* ---- Appointment struct ---- */
typedef struct {
    char appointmentID[9];
    char patientName[50];
    char doctorName[50];
    char date[11];
    char type[20]; /* Pregnant | Senior | PWD | Regular */
    char status[15];   /* Scheduled | Cancelled | Completed | Missed */
}  Appointment ;

#endif /* APPTSTRUCTURE_H */