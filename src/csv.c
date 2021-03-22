#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void csv_setup(){
    FILE *file;

    file = fopen("data.csv", "w");
    fprintf(file, "DATETIME, TI, TE, TR, INTENSITY\n");
    fclose(file);
}

char* get_current_time() {
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
}

void write_csv(float TR, float TE, float TI, float intensity) {
    FILE *file;
    file = fopen("data.csv", "a");

    fprintf(file, "%s, %f, %lf, %f, %lf\n", get_current_time(), TI, TE, TR, intensity);
    fclose(file);
}