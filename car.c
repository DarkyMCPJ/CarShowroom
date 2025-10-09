#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "car.h"
#include "fileio.h"

extern int loadAllCars(const char *filename, Car **cars);
extern void saveAllCars(const char *filename, Car *cars, int count);
extern void initFileIfMissing(const char *filename);

static void trim_newline(char *s) {
    s[strcspn(s, "\n")] = 0;
}

void str_to_lower(char *s) {
    for (; *s; ++s)
        *s = tolower(*s);
}

void addCarInteractive(const char *filename) {
    initFileIfMissing(filename);
    Car *cars = NULL;
    int count = loadAllCars(filename, &cars);
    Car c;
    char buf[64];

    printf("Enter Car Model (or press Enter to cancel): ");
    if (!fgets(c.model, MAX_MODEL, stdin)) { free(cars); return; }
    trim_newline(c.model);
    if (strlen(c.model) == 0) {
        printf("❎ Cancelled add operation.\n");
        free(cars);
        return;
    }

    for (;;) {
        printf("Enter Year (1886-2025): ");
        fgets(buf, sizeof(buf), stdin);
        int year = atoi(buf);
        if (year >= 1886 && year <= 2025) {
            c.year = year;
            break;
        }
        printf("❌ Invalid year. Must be between 1886 and 2025.\n");
    }

    int duplicateIndex = -1;
    for (int i = 0; i < count; i++) {
        if (strcasecmp(cars[i].model, c.model) == 0 && cars[i].year == c.year) {
            duplicateIndex = i;
            break;
        }
    }

    if (duplicateIndex != -1) {
        printf("⚠️ Car '%s' (%d) already exists.\n", c.model, c.year);
        printf("Do you want to update it instead? (y/n): ");
        fgets(buf, sizeof(buf), stdin);
        if (tolower(buf[0]) == 'y') {
            printf("Enter new Price (current %.2f): ", cars[duplicateIndex].price);
            fgets(buf, sizeof(buf), stdin); trim_newline(buf);
            if (strlen(buf)) cars[duplicateIndex].price = atof(buf);
            for (;;) {
                printf("Enter new Availability (Yes/No, current %s): ", cars[duplicateIndex].availability);
                fgets(buf, sizeof(buf), stdin); trim_newline(buf);
                if (strlen(buf) == 0) break;
                if (strcasecmp(buf, "Yes") == 0 || strcasecmp(buf, "No") == 0) {
                    strncpy(cars[duplicateIndex].availability, buf, sizeof(cars[duplicateIndex].availability));
                    break;
                }
                printf("❌ Invalid availability. Please enter 'Yes' or 'No'.\n");
            }
            saveAllCars(filename, cars, count);
            printf("✅ Existing car updated successfully!\n");
        } else {
            printf("❎ Add operation cancelled (duplicate found).\n");
        }
        free(cars);
        return;
    }

    int availability_is_set = 0;
    for (;;) {
        printf("Enter Price: ");
        fgets(buf, sizeof(buf), stdin);
        trim_newline(buf);
        float p = atof(buf);
        if (p > 0) {
            c.price = p;
            break;
        } else if (p == 0) {
            printf("⚠️ Are you sure? Setting the price to 0 will automatically set availability to \"No\". (y/n): ");
            fgets(buf, sizeof(buf), stdin);
            if (tolower(buf[0]) == 'y') {
                c.price = 0.0;
                strcpy(c.availability, "No");
                availability_is_set = 1;
                break;
            } else {
                printf("Price entry cancelled. Please enter a different price.\n");
                continue;
            }
        } else {
            printf("❌ Invalid price. Try again.\n");
        }
    }

    if (!availability_is_set) {
        for (;;) {
            printf("Enter Availability (Yes/No): ");
            fgets(c.availability, sizeof(c.availability), stdin);
            trim_newline(c.availability);
            if (strcasecmp(c.availability, "Yes") == 0 || strcasecmp(c.availability, "No") == 0)
                break;
            printf("❌ Invalid availability. Please enter 'Yes' or 'No'.\n");
        }
    }

    FILE *fp = fopen(filename, "a");
    if (!fp) { perror("File open failed"); free(cars); return; }
    fprintf(fp, "%s,%d,%.2f,%s\n", c.model, c.year, c.price, c.availability);
    fclose(fp);
    free(cars);
    printf("✅ Car added successfully!\n");
}

void displayCars(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) { perror("File open failed"); return; }
    char line[MAX_LINE];
    int first = 1;
    printf("\n%-20s %-6s %-10s %-12s\n", "Car Model", "Year", "Price", "Availability");
    printf("---------------------------------------------------------------\n");
    while (fgets(line, sizeof(line), fp)) {
        if (first) {
            first = 0;
            continue;
        }
        Car c;
        sscanf(line, "%[^,],%d,%f,%[^\n]", c.model, &c.year, &c.price, c.availability);
        printf("%-20s %-6d %-10.2f %-12s\n", c.model, c.year, c.price, c.availability);
    }
    fclose(fp);
}

void searchCarInteractive(const char *filename) {
    char keyword[MAX_MODEL];
    printf("Enter Car Model or Year to search (or press Enter to cancel): ");
    if (!fgets(keyword, sizeof(keyword), stdin)) return;
    trim_newline(keyword);
    if (strlen(keyword) == 0) {
        printf("❎ Cancelled search.\n");
        return;
    }
    char keyword_lower[MAX_MODEL];
    strncpy(keyword_lower, keyword, MAX_MODEL);
    str_to_lower(keyword_lower);
    Car *cars = NULL;
    int n = loadAllCars(filename, &cars);
    if (n < 0) { perror("File error"); return; }
    Car *modelMatches = malloc(sizeof(Car) * n);
    Car *yearMatches = malloc(sizeof(Car) * n);
    int mModel = 0, mYear = 0;
    for (int i = 0; i < n; ++i) {
        char yearStr[16];
        sprintf(yearStr, "%d", cars[i].year);
        char model_lower[MAX_MODEL];
        strncpy(model_lower, cars[i].model, MAX_MODEL);
        str_to_lower(model_lower);
        if (strstr(model_lower, keyword_lower)) {
            modelMatches[mModel++] = cars[i];
        } else if (strstr(yearStr, keyword_lower)) {
            yearMatches[mYear++] = cars[i];
        }
    }
    if (mModel + mYear == 0) {
        printf("No cars found for '%s'.\n", keyword);
    } else {
        printf("\n%-25s %-6s %-10s %-12s\n", "Car Model", "Year", "Price", "Availability");
        printf("-----------------------------------------------------------------\n");
        for (int i = 0; i < mModel; ++i)
            printf("%-25s %-6d %-10.2f %-12s\n", modelMatches[i].model, modelMatches[i].year, modelMatches[i].price, modelMatches[i].availability);
        for (int i = 0; i < mYear; ++i)
            printf("%-25s %-6d %-10.2f %-12s\n", yearMatches[i].model, yearMatches[i].year, yearMatches[i].price, yearMatches[i].availability);
    }
    free(cars);
    free(modelMatches);
    free(yearMatches);
}

void updateCarInteractive(const char *filename) {
    char keyword[MAX_MODEL];
    printf("Enter car model or year to update (or press Enter to cancel): ");
    if (!fgets(keyword, sizeof(keyword), stdin)) return;
    trim_newline(keyword);
    if (strlen(keyword) == 0) {
        printf("❎ Cancelled update operation.\n");
        return;
    }
    char keyword_lower[MAX_MODEL];
    strncpy(keyword_lower, keyword, MAX_MODEL);
    str_to_lower(keyword_lower);
    Car *cars = NULL;
    int n = loadAllCars(filename, &cars);
    if (n < 0) { perror("File error"); return; }
    int *matches = malloc(sizeof(int) * n);
    int mcount = 0;
    for (int i = 0; i < n; ++i) {
        char yearStr[16];
        sprintf(yearStr, "%d", cars[i].year);
        char model_lower[MAX_MODEL];
        strncpy(model_lower, cars[i].model, MAX_MODEL);
        str_to_lower(model_lower);
        if (strstr(model_lower, keyword_lower) || strstr(yearStr, keyword_lower))
            matches[mcount++] = i;
    }

    if (mcount == 0) {
        printf("No matches found for '%s'.\n", keyword);
        free(cars);
        free(matches);
        return;
    } // <-- THE MISSING BRACE GOES HERE

    int chosen = -1;
    if (mcount == 1) {
        printf("Found 1 match: %s (%d, %.2f, %s)\n", cars[matches[0]].model, cars[matches[0]].year, cars[matches[0]].price, cars[matches[0]].availability);
        printf("Update this car? (y/n): ");
        char confirm[8];
        fgets(confirm, sizeof(confirm), stdin);
        if (tolower(confirm[0]) != 'y') {
            printf("❎ Cancelled update.\n");
            free(cars);
            free(matches);
            return;
        }
        chosen = matches[0];
    } else {
        printf("Multiple matches:\n");
        for (int i = 0; i < mcount; ++i) {
            int idx = matches[i];
            printf("%d) %s (%d, %.2f, %s)\n", i + 1, cars[idx].model, cars[idx].year, cars[idx].price, cars[idx].availability);
        }
        printf("Select number to update (0 to cancel): ");
        char buf[16];
        fgets(buf, sizeof(buf), stdin);
        int sel = atoi(buf);
        if (sel == 0) {
            printf("❎ Cancelled update operation.\n");
            free(cars);
            free(matches);
            return;
        }
        if (sel < 1 || sel > mcount) {
            printf("Invalid selection.\n");
            free(cars);
            free(matches);
            return;
        }
        chosen = matches[sel - 1];
    }

    printf("\nUpdating: %s\n", cars[chosen].model);
    char buf[128];
    int availability_overridden = 0;
    printf("Enter new model (blank to keep): ");
    fgets(buf, sizeof(buf), stdin);
    trim_newline(buf);
    if (strlen(buf)) strncpy(cars[chosen].model, buf, MAX_MODEL);

    while (1) {
        printf("Enter new year (1886-2025, blank to keep): ");
        fgets(buf, sizeof(buf), stdin);
        trim_newline(buf);
        if (strlen(buf) == 0) break;
        int year = atoi(buf);
        if (year >= 1886 && year <= 2025) {
            cars[chosen].year = year;
            break;
        }
        printf("❌ Invalid year. Must be between 1886 and 2025.\n");
    }

    printf("Enter new price (blank to keep): ");
    fgets(buf, sizeof(buf), stdin);
    trim_newline(buf);
    if (strlen(buf)) {
        float p = atof(buf);
        if (p > 0) {
            cars[chosen].price = p;
        } else if (p == 0) {
            printf("⚠️ Are you sure? Setting the price to 0 will automatically set availability to \"No\". (y/n): ");
            fgets(buf, sizeof(buf), stdin);
            if (tolower(buf[0]) == 'y') {
                cars[chosen].price = 0.0;
                strcpy(cars[chosen].availability, "No");
                availability_overridden = 1;
                printf("Price set to 0 and availability set to 'No'.\n");
            } else {
                printf("Price update cancelled.\n");
            }
        } else {
            printf("❌ Invalid price entered. Price not updated.\n");
        }
    }
    
    if (!availability_overridden) {
        while (1) {
            printf("Enter new availability (Yes/No, blank to keep): ");
            fgets(buf, sizeof(buf), stdin);
            trim_newline(buf);
            if (strlen(buf) == 0) break;
            if (strcasecmp(buf, "Yes") == 0 || strcasecmp(buf, "No") == 0) {
                strncpy(cars[chosen].availability, buf, sizeof(cars[chosen].availability));
                break;
            }
            printf("Invalid availability (please enter 'Yes' or 'No').\n");
        }
    }

    saveAllCars(filename, cars, n);
    printf("✅ Car updated successfully.\n");
    free(matches);
    free(cars);
}

void deleteCarInteractive(const char *filename) {
    char keyword[MAX_MODEL];
    printf("Enter keyword of Car Model to delete (or press Enter to cancel): ");
    if (!fgets(keyword, sizeof(keyword), stdin)) return;
    trim_newline(keyword);
    if (strlen(keyword) == 0) {
        printf("❎ Cancelled delete operation.\n");
        return;
    }
    char keyword_lower[MAX_MODEL];
    strncpy(keyword_lower, keyword, MAX_MODEL);
    str_to_lower(keyword_lower);
    Car *cars = NULL;
    int n = loadAllCars(filename, &cars);
    if (n < 0) { perror("File error"); return; }
    int *matches = malloc(sizeof(int) * n);
    int mcount = 0;
    for (int i = 0; i < n; ++i) {
        char model_lower[MAX_MODEL];
        strncpy(model_lower, cars[i].model, MAX_MODEL);
        str_to_lower(model_lower);
        if (strstr(model_lower, keyword_lower)) {
            matches[mcount++] = i;
        }
    }

    if (mcount == 0) {
        printf("No matches found for '%s'.\n");
        free(cars);
        free(matches);
        return;
    }

    int chosenIdx = -1;
    if (mcount == 1) {
        printf("Found 1 match: %s (%d, %.2f, %s)\n", cars[matches[0]].model, cars[matches[0]].year, cars[matches[0]].price, cars[matches[0]].availability);
        printf("Delete this car? (y/n): ");
        char confirm[8];
        fgets(confirm, sizeof(confirm), stdin);
        if (tolower(confirm[0]) != 'y') {
            printf("❎ Cancelled delete.\n");
            free(cars);
            free(matches);
            return;
        }
        chosenIdx = matches[0];
    } else {
        printf("Multiple matches:\n");
        for (int i = 0; i < mcount; ++i) {
            int idx = matches[i];
            printf("%d) %s (%d, %.2f, %s)\n", i + 1, cars[idx].model, cars[idx].year, cars[idx].price, cars[idx].availability);
        }
        printf("Select number to delete (0 to cancel): ");
        char buf[16];
        fgets(buf, sizeof(buf), stdin);
        int sel = atoi(buf);
        if (sel == 0) {
            printf("❎ Cancelled delete operation.\n");
            free(cars);
            free(matches);
            return;
        }
        if (sel < 1 || sel > mcount) {
            printf("❌ Invalid selection.\n");
            free(cars);
            free(matches);
            return;
        }
        chosenIdx = matches[sel - 1];
    }

    char deletedModel[MAX_MODEL];
    strcpy(deletedModel, cars[chosenIdx].model);
    for (int i = chosenIdx; i < n - 1; ++i)
        cars[i] = cars[i + 1];
    saveAllCars(filename, cars, n - 1);
    printf("✅ Car deleted: %s\n", deletedModel);
    free(cars);
    free(matches);
}

int isCarDataValid(const Car* car) {
    if (car->year < 1886 || car->year > 2025) {
        return 0;
    }
    if (car->price < 0) {
        return 0;
    }
    if (strlen(car->model) == 0) {
        return 0;
    }
    if (strcasecmp(car->availability, "Yes") != 0 && strcasecmp(car->availability, "No") != 0) {
        return 0;
    }
    return 1;
}
