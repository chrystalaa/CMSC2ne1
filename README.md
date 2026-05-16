# CMSC2ne1
Final Project for CMSC 21

# Clinic Management System
A terminal-based clinic management system written in C, developed as a final project for CMSC 21.

**Group:** CMSC2ne1

## Features
- **Appointment Management** — Book, view, edit, and cancel appointments
- **Queue Management** — Manage patient check-ins and queue order
- **Patient Management** — Add, search, view, and delete patient records
- **Doctor Management** — Add, delete, and view doctor schedules

## Dependencies
- GCC
- [Raylib](https://www.raylib.com/) — for the GUI

## How to Compile
```bash
gcc main-clinic.c -o clinic -Wall -lraylib
```

## How to Run
```bash
./clinic        # Linux/Mac
clinic.exe      # Windows
```

## Navigation
- Enter the number of your chosen option at any menu
- Enter `0` at any prompt to go back one level

## File Structure
project/
├── main-clinic.c
├── AppointmentManagement/
│   ├── core/
│   ├── filehandling/
│   ├── helpers/
│   └── crud/
├── DoctorManagement/
├── PatientManagement/
│   ├── core/
│   └── PatientMgmtFunctions/ (CRUD) 
└── QueueManagement/

## Data Files (auto-generated on first run)
- `appointments.txt` — Appointment records
- `doctors.txt` — Doctor records (seeded with 5 defaults)
- `patients.txt` — Patient records
- `appt_counter.txt` — Appointment ID counter

## Authors
- Honorario, Matthew 
- Mendoza, Chrystel Therese
- Narca, Hannah Zoey
- Niebla, Samuel
- Sinugbuhan, Kevin






