#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "car.h"
#include "fileio.h"

extern int loadAllCars(const char *filename, Car **cars);
extern void saveAllCars(const char *filename, Car *cars, int count);

void runUnitTests(void) {
    const char *testFile = "test_unit.csv";
    printf("\n===== Unit Test Menu =====\n");

    FILE *fp = fopen(testFile, "w");
    if (!fp) {
        perror("Failed to open test file");
        return;
    }
    fprintf(fp, "CarModel,Year,Price,Availability\n");
    fprintf(fp, "Honda,2019,20000,Yes\n");
    fprintf(fp, "Toyota,2021,25000,No\n");
    fclose(fp);
    printf("✅ Test file '%s' has been reset with default data.\n", testFile);

    int choice;
    for (;;) {
        printf("\nSelect a function to test:\n");
        printf("1. loadAllCars\n");
        printf("2. addCarInteractive\n");
        printf("3. saveAllCars\n");
        printf("4. displayCars\n");
        printf("0. Return to main menu\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // clear invalid input
            printf("Invalid input. Try again.\n");
            continue;
        }
        getchar(); // consume newline

        if (choice == 0) {
            printf("Returning to main menu...\n");
            break;
        }

        Car *cars = NULL;
        int n;

        switch (choice) {
            case 1:
                printf("\n--- Testing loadAllCars ---\n");
                n = loadAllCars(testFile, &cars);
                printf("loadAllCars returned %d car(s)\n", n);
                free(cars);
                break;
            case 2:
                printf("\n--- Testing addCarInteractive ---\n");
                addCarInteractive(testFile);
                printf("Check file '%s' for added car.\n", testFile);
                break;
            case 3:
                printf("\n--- Testing saveAllCars ---\n");
                n = loadAllCars(testFile, &cars);
                if (n > 0) {
                    cars[0].price += 1000;
                    saveAllCars(testFile, cars, n);
                    printf("saveAllCars executed, first car price increased by 1000.\n");
                } else {
                    printf("No cars to save.\n");
                }
                free(cars);
                break;
            case 4:
                printf("\n--- Testing displayCars ---\n");
                displayCars(testFile);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}