#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "car.h"
#include "fileio.h"

//Color definitions
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED   "\033[31m"
#define BLUE  "\033[34m"
#define CYAN  "\033[36m"

static int tests_passed;
static int tests_total;

void assert_equal_int(int expected, int actual, const char* message) {
    tests_total++;
    if (expected == actual) {
        printf(GREEN "  ✓ PASS: " RESET "%s (Expected: %d, Got: %d)\n", message, expected, actual);
        tests_passed++;
    } else {
        printf(RED "  ✗ FAIL: " RESET "%s (Expected: %d, Got: %d)\n", message, expected, actual);
    }
}



// UNIT TESTS

void runUnitTests(void) {
    tests_passed = 0;
    tests_total = 0;

    printf(BLUE "\n=== Running Unit Tests ===" RESET "\n");
    
    const char *testFile = "test_unit.csv";

    initFileIfMissing(testFile);
    FILE *fp = fopen(testFile, "r");
    assert_equal_int(1, fp != NULL, "File should be created by initFileIfMissing");
    if (fp) fclose(fp);


    Car carsToWrite[1] = {{"Test Model", 2025, 9999.0, "Yes"}};
    saveAllCars(testFile, carsToWrite, 1);
    
    Car *loadedCars = NULL;
    int count = loadAllCars(testFile, &loadedCars);
    assert_equal_int(1, count, "Should load 1 car from the file");
    if(count == 1) {
       assert_equal_int(0, strcmp(loadedCars[0].model, "Test Model"), "Loaded car model should match saved model");
    }
    
    free(loadedCars);
    remove(testFile);

    printf(CYAN "\n--- Unit Test Summary --- \n" RESET);
    if (tests_passed == tests_total) printf(GREEN "ALL %d UNIT TESTS PASSED!\n" RESET, tests_total);
    else printf(RED "%d / %d UNIT TESTS PASSED\n" RESET, tests_passed, tests_total);
    printf("-------------------------\n");
}

// END-TO-END TEST

int find_car_by_model_test(Car* cars, int count, const char* model) {
    for (int i = 0; i < count; i++) {
        if (strcmp(cars[i].model, model) == 0) return i;
    }
    return -1;
}

void runE2ETests(void) {
    tests_passed = 0;
    tests_total = 0;
    
    printf(BLUE "\n=== Running E2E Workflow Test ===" RESET "\n");
    const char* test_filename = "test_e2e_workflow.csv";
    remove(test_filename); 


    Car initial_cars[] = {
        {"Toyota Camry", 2021, 25000.00, "Yes"},
        {"Ford Focus", 2019, 19000.00, "No"}
    };
    saveAllCars(test_filename, initial_cars, 2);


    Car* cars = NULL;
    int count = loadAllCars(test_filename, &cars);

    int focus_idx = find_car_by_model_test(cars, count, "Ford Focus");
    if (focus_idx != -1) {
        for (int i = focus_idx; i < count - 1; i++) cars[i] = cars[i+1];
        count--;
    }
    saveAllCars(test_filename, cars, count);
    free(cars);
    
   
    Car* final_cars = NULL;
    int final_count = loadAllCars(test_filename, &final_cars);
    
    assert_equal_int(1, final_count, "Final car count should be 1 after deletion");
    int final_focus_idx = find_car_by_model_test(final_cars, final_count, "Ford Focus");
    assert_equal_int(-1, final_focus_idx, "Ford Focus should be deleted");
    
    free(final_cars);
    remove(test_filename);

    printf(CYAN "\n--- E2E Test Summary --- \n" RESET);
    if (tests_passed == tests_total) printf(GREEN "ALL %d E2E TESTS PASSED!\n" RESET, tests_total);
    else printf(RED "%d / %d E2E TESTS PASSED\n" RESET, tests_passed, tests_total);
    printf("------------------------\n");
}
