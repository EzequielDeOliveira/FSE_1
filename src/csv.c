#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char buffer[26];

void csv_setup()
{
    FILE *file;

    file = fopen("data.csv", "w");
    fprintf(file, "DATETIME, TI, TE, TR, INTENSITY\n");
    fclose(file);
}

void get_current_time()
{
    time_t timer;
    struct tm *tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

void write_csv(float TR, float TE, float TI, float intensity)
{
    FILE *file;
    file = fopen("data.csv", "a");

    get_current_time();

    fprintf(file, "%s, %f, %lf, %f, %lf\n", buffer, TI, TE, TR, intensity);
    fclose(file);
}