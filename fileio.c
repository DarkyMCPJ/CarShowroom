#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "car.h"

void copyFile(const char *src, const char *dest) {
    FILE *fs = fopen(src, "r");
    FILE *fd = fopen(dest, "w");
    if (!fs || !fd) {
        perror("File copy failed");
        if (fs) fclose(fs);
        if (fd) fclose(fd);
        return;
    }
    char ch;
    while ((ch = fgetc(fs)) != EOF)
        fputc(ch, fd);
    fclose(fs);
    fclose(fd);
}

void initFileIfMissing(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fp = fopen(filename, "w");
        if (fp) {
            fprintf(fp, "CarModel,Year,Price,Availability\n");
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
}

int loadAllCars(const char *filename, Car **cars) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[MAX_LINE];
    int count = 0, first = 1;
    *cars = NULL;

    while (fgets(line, sizeof(line), fp)) {
        if (first) {
            first = 0;
            continue;
        }
        *cars = realloc(*cars, sizeof(Car) * (count + 1));
        sscanf(line, "%[^,],%d,%f,%[^\n]",
               (*cars)[count].model, &(*cars)[count].year,
               &(*cars)[count].price, (*cars)[count].availability);
        count++;
    }
    fclose(fp);
    return count;
}

void saveAllCars(const char *filename, Car *cars, int count) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    fprintf(fp, "CarModel,Year,Price,Availability\n");
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%d,%.2f,%s\n",
                cars[i].model, cars[i].year, cars[i].price, cars[i].availability);
    }
    fclose(fp);
}
