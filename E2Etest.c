#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "car.h"
#include "fileio.h"

void runE2ETests(void) {
    const char *testFile = "test_e2e.csv";
    printf("\n===== E2E Test Menu =====\n");

    FILE *fp = fopen(testFile, "w");
    if (!fp) {
        perror("Failed to open E2E test file");
        return;
    }
    fprintf(fp, "CarModel,Year,Price,Availability\n");
    fprintf(fp, "Honda Civic,2019,20000,Yes\n");
    fprintf(fp, "Toyota Camry,2021,25000,No\n");
    fprintf(fp, "Ford Focus,2018,18000,Yes\n");
    fclose(fp);
    printf("✅ E2E test file '%s' reset with sample data.\n", testFile);

    int choice;
    for (;;) {
        printf("\nSelect an E2E scenario:\n");
        printf("1. Display initial data\n");
        printf("2. Update Honda Civic (price +1000, set No)\n");
        printf("3. Delete Ford Focus\n");
        printf("4. Add a new car interactively\n");
        printf("5. Display final data\n");
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
                printf("\n--- Initial Data ---\n");
                displayCars(testFile);
                break;
            case 2:
                printf("\n--- Updating Honda Civic ---\n");
                n = loadAllCars(testFile, &cars);
                for (int i = 0; i < n; ++i) {
                    if (strcasecmp(cars[i].model, "Honda Civic") == 0) {
                        cars[i].price += 1000;
                        strcpy(cars[i].availability, "No");
                    }
                }
                saveAllCars(testFile, cars, n);
                printf("✅ Honda Civic updated successfully.\n");
                free(cars);
                break;
            case 3:
                printf("\n--- Deleting Ford Focus ---\n");
                n = loadAllCars(testFile, &cars);
                {
                    Car *temp_cars = malloc(sizeof(Car) * n);
                    int keep = 0;
                    for (int i = 0; i < n; ++i) {
                        if (strcasecmp(cars[i].model, "Ford Focus") != 0) {
                            temp_cars[keep++] = cars[i];
                        }
                    }
                    saveAllCars(testFile, temp_cars, keep);
                    free(temp_cars);
                    printf("✅ Ford Focus deleted successfully.\n");
                }
                free(cars);
                break;
            case 4:
                printf("\n--- Adding new car interactively ---\n");
                addCarInteractive(testFile);
                break;
            case 5:
                printf("\n--- Final Data ---\n");
                displayCars(testFile);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    printf("✅ E2E Test Completed. File '%s' kept for inspection.\n", testFile);
}