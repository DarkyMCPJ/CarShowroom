#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

static int is_line_empty(const char *s) {
    while (*s != '\0') {
        if (!isspace((unsigned char)*s))
            return 0; 
        s++;
    }
    return 1; 
}

int loadAllCars(const char *filename, Car **cars) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        *cars = NULL;
        return 0; 
    }

    char line[MAX_LINE];
    int count = 0;
    *cars = NULL;


    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return 0; 
    }

    while (fgets(line, sizeof(line), fp)) {

        if (is_line_empty(line)) {
            continue; 
        }

        Car* temp = realloc(*cars, sizeof(Car) * (count + 1));
        if (temp == NULL) {
            perror("Failed to reallocate memory for cars");
            free(*cars);
            fclose(fp);
            return -1; 
        }
        *cars = temp;
        
        int items_scanned = sscanf(line, "%49[^,],%d,%f,%9[^\n]",
               (*cars)[count].model, &(*cars)[count].year,
               &(*cars)[count].price, (*cars)[count].availability);

        if (items_scanned >= 4) {
            count++; 
        } else {
            printf("Warning: Skipped corrupted or malformed line: %s", line);
        }
    }
    fclose(fp);
    

    if (count == 0) {
        free(*cars);
        *cars = NULL;
    }
    
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
