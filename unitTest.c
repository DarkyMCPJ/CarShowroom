#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "car.h"
#include "fileio.h"

void runUnitTests(void) {
    const char *srcFile = "testdata.csv";
    const char *testFile = "test_unit.csv";

    copyFile(srcFile, testFile);
    printf("✅ Copied data from '%s' to '%s'\n", srcFile, testFile);

    int choice = -1;

    for (;;) {
        printf("\n===== Unit Test Menu =====\n");
        printf("1. Display all cars\n");
        printf("2. Add new car\n");
        printf("3. Update price of first car\n");
        printf("4. Save cars\n");
        printf("0. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        getchar();

        Car *cars = NULL;
        int count = loadAllCars(testFile, &cars);

        switch (choice) {
            case 1:
                displayCars(testFile);
                break;
            case 2:
                addCarInteractive(testFile);
                break;
            case 3:
                if (count > 0) {
                    cars[0].price += 1000;
                    saveAllCars(testFile, cars, count);
                    printf("Updated first car’s price +1000\n");
                }
                break;
            case 4:
                saveAllCars(testFile, cars, count);
                printf("Saved successfully.\n");
                break;
            case 0:
                printf("Returning...\n");
                free(cars);
                return;
        }

        free(cars);
    }
}
