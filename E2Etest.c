#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "car.h"
#include "fileio.h"


// Main E2E test function
void runE2ETests(void) {
    const char *srcFile = "testdata.csv";   // your master dataset
    const char *testFile = "test_e2e.csv";  // working copy

    copyFile(srcFile, testFile);
    printf("✅ Copied dataset from '%s' → '%s'\n", srcFile, testFile);

    int choice = -1;

    for (;;) {
        printf("\n===== E2E Test Menu =====\n");
        printf("1. Display all cars\n");
        printf("2. Add new car interactively\n");
        printf("3. Update Honda Civic (price +1000, set No)\n");
        printf("4. Delete Ford Focus\n");
        printf("5. Display final list\n");
        printf("6. Reset from testdata.csv\n");
        printf("0. Return to main menu\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        getchar();

        if (choice == 0) {
            printf("Returning to main menu...\n");
            break;
        }

        Car *cars = NULL;
        int count = loadAllCars(testFile, &cars);
        if (count < 0) {
            printf("Failed to load data.\n");
            continue;
        }

        switch (choice) {
            case 1:
                printf("\n--- Displaying all cars ---\n");
                displayCars(testFile);
                break;

            case 2:
                printf("\n--- Adding new car ---\n");
                addCarInteractive(testFile);
                break;

            case 3:
                printf("\n--- Updating Honda Civic ---\n");
                for (int i = 0; i < count; ++i) {
                    if (strcasecmp(cars[i].model, "Honda Civic") == 0) {
                        cars[i].price += 1000;
                        strcpy(cars[i].availability, "No");
                        printf("Updated: %s → price=%.2f, availability=No\n", 
                               cars[i].model, cars[i].price);
                    }
                }
                saveAllCars(testFile, cars, count);
                printf("✅ Honda Civic updated.\n");
                break;

            case 4:
                printf("\n--- Deleting Ford Focus ---\n");
                {
                    int keep = 0;
                    for (int i = 0; i < count; ++i) {
                        if (strcasecmp(cars[i].model, "Ford Focus") != 0)
                            cars[keep++] = cars[i];
                    }
                    saveAllCars(testFile, cars, keep);
                    printf("✅ Ford Focus deleted. (%d cars remain)\n", keep);
                }
                break;

            case 5:
                printf("\n--- Displaying final list ---\n");
                displayCars(testFile);
                break;

            case 6:
                copyFile(srcFile, testFile);
                printf("✅ Reset '%s' from '%s'\n", testFile, srcFile);
                break;

            default:
                printf("Invalid choice.\n");
        }

        free(cars);
    }

    printf("✅ E2E test completed. File '%s' kept for review.\n", testFile);
}
