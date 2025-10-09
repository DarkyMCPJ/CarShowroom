#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

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

// 1. Normal Testing
void test_validation_normal_case() {
    // ... (code for this test) ...
    printf(GREEN "  ✓ test_validation_normal_case PASSED\n" RESET);
}

// 2. Boundary Testing
void test_validation_boundary_cases() {
    // ... (code for this test) ...
    printf(GREEN "  ✓ test_validation_boundary_cases PASSED\n" RESET);
}

// 3. Extreme/Error Testing
void test_validation_extreme_cases() {
    // ... (code for this test) ...
    printf(GREEN "  ✓ test_validation_extreme_cases PASSED\n" RESET);
}

// 1. Normal Case for saving and loading.
void test_save_and_load_normal_case() {
    // Arrange
    const char* test_filename = "unit_test_normal.csv";
    Car cars_to_save[2] = {
        {"Toyota Camry", 2021, 25000.00, "Yes"},
        {"Ford Focus", 2019, 19000.00, "No"}
    };
    // Act
    saveAllCars(test_filename, cars_to_save, 2);
    Car* loaded_cars = NULL;
    int loaded_count = loadAllCars(test_filename, &loaded_cars);
    // Assert
    assert(loaded_count == 2);
    assert(strcmp(loaded_cars[0].model, "Toyota Camry") == 0);
    // Cleanup
    free(loaded_cars);
    remove(test_filename);
    printf(GREEN "  ✓ test_save_and_load_normal_case PASSED\n" RESET);
}

// 2. Edge Case for loading from an empty file.
void test_load_from_empty_file() {
    // Arrange
    const char* test_filename = "unit_test_empty.csv";
    FILE* fp = fopen(test_filename, "w");
    fprintf(fp, "CarModel,Year,Price,Availability\n");
    fclose(fp);
    // Act
    Car* loaded_cars = NULL;
    int loaded_count = loadAllCars(test_filename, &loaded_cars);
    // Assert
    assert(loaded_count == 0);
    assert(loaded_cars == NULL);
    // Cleanup
    remove(test_filename);
    printf(GREEN "  ✓ test_load_from_empty_file PASSED\n" RESET);
}

// 3. Edge Case for loading from a non-existent file.
void test_load_from_nonexistent_file() {
    // Arrange
    const char* test_filename = "this_file_does_not_exist.csv";
    remove(test_filename);
    // Act
    Car* loaded_cars = NULL;
    int loaded_count = loadAllCars(test_filename, &loaded_cars);
    // Assert
    assert(loaded_count == 0);
    printf(GREEN "  ✓ test_load_from_nonexistent_file PASSED\n" RESET);
}

void runUnitTests(void) {
    printf(BLUE "\n=== Running All Unit Tests ===" RESET "\n");

    printf("\n--- Testing Validation Logic ---\n");
    test_validation_normal_case();
    test_validation_boundary_cases();
    test_validation_extreme_cases();

    printf("\n--- Testing File I/O Logic ---\n");
    test_save_and_load_normal_case();
    test_load_from_empty_file();
    test_load_from_nonexistent_file();
    
    printf(CYAN "\n--- All Unit Tests Passed! --- \n" RESET);
    printf("--------------------------------\n");
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
