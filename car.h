#ifndef CAR_H
#define CAR_H

#define FILENAME "cars.csv"
#define MAX_MODEL 50
#define MAX_LINE 256

typedef struct {
    char model[MAX_MODEL];
    int year;
    float price;
    char availability[10];
} Car;

void addCarInteractive(const char *filename);
void displayCars(const char *filename);
void searchCarInteractive(const char *filename);
void updateCarInteractive(const char *filename);
void deleteCarInteractive(const char *filename);

void runUnitTests(void);
void runE2ETests(void);

#endif