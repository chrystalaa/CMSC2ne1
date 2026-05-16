#ifndef DATEPICKER_H
#define DATEPICKER_H

#include <stdio.h>
#include <time.h>


static inline int isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static inline int daysInMonth(int m, int y) {
    int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && isLeapYear(y)) return 29;
    return days[m - 1];
}

/*
 * Fills dateOut (>= 11 chars) with YYYY-MM-DD.
 * Returns 1 on success, 0 if user cancelled at any step.
 */
static inline int pickDate(char *dateOut) {
    time_t now = time(NULL);
    now += 8 * 3600;              /* shift to UTC+8 (Philippine Time) */
    struct tm *t = gmtime(&now);  /* use gmtime since we manually shifted */
    int ty = t->tm_year + 1900;
    int tm = t->tm_mon  + 1;
    int td = t->tm_mday;

    int y, m, d;

    /* ── YEAR ─────────────────────────────────────────────── */
    while (1) {
        printf("\n--- Select Year ---\n");
        for (int i = 0; i < 3; i++)
            printf("  %d. %d\n", i + 1, ty + i);
        printf("  0. Cancel\n");
        printf("Choice: ");

        int ch;
        if (scanf("%d", &ch) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');

        if (ch == 0) { printf("Going back.\n"); return 0; }
        if (ch >= 1 && ch <= 3) { y = ty + ch - 1; break; }
        printf("Invalid choice.\n");
    }

    /* ── MONTH ────────────────────────────────────────────── */
    const char *MON[] = {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };
    int startMonth = (y == ty) ? tm : 1;

    while (1) {
        printf("\n--- Select Month ---\n");
        int idx = 1, monthMap[13] = {0};
        for (int mo = startMonth; mo <= 12; mo++) {
            printf("  %2d. %s\n", idx, MON[mo - 1]);
            monthMap[idx++] = mo;
        }
        printf("   0. Cancel\n");
        printf("Choice: ");

        int ch;
        if (scanf("%d", &ch) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');

        if (ch == 0) { printf("Going back.\n"); return 0; }
        if (ch >= 1 && ch < idx) { m = monthMap[ch]; break; }
        printf("Invalid choice.\n");
    }

    /* ── DAY ──────────────────────────────────────────────── */
    int maxDay   = daysInMonth(m, y);
    int startDay = (y == ty && m == tm) ? td : 1;

    while (1) {
        printf("\n--- Select Day ---\n");
        int idx = 1, dayMap[32] = {0};
        for (int day = startDay; day <= maxDay; day++) {
            printf("  %2d. %02d", idx, day);
            if (idx % 7 == 0) printf("\n");   /* 7 per row */
            dayMap[idx++] = day;
        }
        printf("\n   0. Cancel\n");
        printf("Choice: ");

        int ch;
        if (scanf("%d", &ch) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');

        if (ch == 0) { printf("Going back.\n"); return 0; }
        if (ch >= 1 && ch < idx) { d = dayMap[ch]; break; }
        printf("Invalid choice.\n");
    }

    snprintf(dateOut, 11, "%04d-%02d-%02d", y, m, d);
    return 1;
}

#endif /* DATEPICKER_H */