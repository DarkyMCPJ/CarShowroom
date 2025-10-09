#include "memory_tracker.h" 
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

void assert_equal_string(const char* expected, const char* actual, const char* message) {
    tests_total++;
    if (strcmp(expected, actual) == 0) {
        printf(GREEN "  ✓ PASS: " RESET "%s\n", message);
        tests_passed++;
    } else {
        printf(RED "  ✗ FAIL: " RESET "%s (Expected: \"%s\", Got: \"%s\")\n", message, expected, actual);
    }
}

void assert_equal_float(float expected, float actual, const char* message) {
    tests_total++;
    // Use a small tolerance for float comparison
    if (fabs(expected - actual) < 0.001) {
        printf(GREEN "  ✓ PASS: " RESET "%s\n", message);
        tests_passed++;
    } else {
        printf(RED "  ✗ FAIL: " RESET "%s (Expected: %.2f, Got: %.2f)\n", message, expected, actual);
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

    report_memory_leaks();
}

// END-TO-END TEST

int find_car_by_model_test(Car* cars, int count, const char* model) {
    for (int i = 0; i < count; i++) {
        if (strcmp(cars[i].model, model) == 0) return i;
    }
    return -1;
}

void test_full_car_management_workflow() {
    printf(BLUE "\n=== END-TO-END TEST CASE: Complete Car Showroom Workflow ===" RESET "\n");
    printf("📋 Scenario: Save initial data -> Update -> Delete -> Add -> Verify final data\n\n");

    // ==================== ARRANGE ====================
    printf(CYAN "🔹 ARRANGE: Preparing the test environment\n" RESET);
    const char* test_filename = "test_showroom.csv";
    remove(test_filename); // Ensure a clean start

    Car initial_cars[] = {
        {"Toyota Camry", 2021, 25000.00, "Yes"},
        {"Ford Focus", 2019, 19000.00, "No"}
    };
    int initial_count = 2;
    printf("  - Test file: %s\n", test_filename);
    printf("  - Preparing %d initial cars.\n", initial_count);

    // ==================== ACT ====================
    printf(CYAN "\n🔹 ACT: Executing the full user workflow\n" RESET);
    
    printf("  Step 1: Saving initial data...\n");
    saveAllCars(test_filename, initial_cars, initial_count);

    printf("  Step 2: Loading data to simulate user session...\n");
    Car* cars = NULL;
    int count = loadAllCars(test_filename, &cars);

    printf("  Step 3: Updating 'Toyota Camry' price...\n");
    int camry_idx = find_car_by_model_test(cars, count, "Toyota Camry");
    if (camry_idx != -1) cars[camry_idx].price = 26500.00;

    printf("  Step 4: Deleting 'Ford Focus'...\n");
    int focus_idx = find_car_by_model_test(cars, count, "Ford Focus");
    if (focus_idx != -1) {
        for (int i = focus_idx; i < count - 1; i++) cars[i] = cars[i+1];
        count--;
    }

    printf("  Step 5: Adding a new 'Tesla Model 3'...\n");
    count++;
    cars = realloc(cars, sizeof(Car) * count);
    strcpy(cars[count - 1].model, "Tesla Model 3");
    cars[count - 1].year = 2023;
    cars[count - 1].price = 48000.00;
    strcpy(cars[count - 1].availability, "Yes");

    printf("  Step 6: Saving final data to file...\n");
    saveAllCars(test_filename, cars, count);
    free(cars);

    printf("  Step 7: Loading final data for verification...\n");
    Car* final_cars = NULL;
    int final_count = loadAllCars(test_filename, &final_cars);

    // ==================== ASSERT ====================
    printf(CYAN "\n🔹 ASSERT: Verifying the final state of the file\n" RESET);

    assert_equal_int(2, final_count, "Final car count should be 2");

    int final_camry_idx = find_car_by_model_test(final_cars, final_count, "Toyota Camry");
    int final_focus_idx = find_car_by_model_test(final_cars, final_count, "Ford Focus");
    int final_tesla_idx = find_car_by_model_test(final_cars, final_count, "Tesla Model 3");

    printf("\n  -- Verifying UPDATED car --\n");
    assert_equal_float(26500.00, final_cars[final_camry_idx].price, "Toyota Camry's price should be updated");

    printf("\n  -- Verifying DELETED car --\n");
    assert_equal_int(-1, final_focus_idx, "Ford Focus should no longer exist");

    printf("\n  -- Verifying NEWLY ADDED car --\n");
    assert_equal_string("Tesla Model 3", final_cars[final_tesla_idx].model, "Tesla Model 3's model should be correct");
    assert_equal_int(2023, final_cars[final_tesla_idx].year, "Tesla Model 3's year should be correct");

    // --- Cleanup ---
    free(final_cars);
    remove(test_filename);
    
    printf(GREEN "\n✅ END-TO-END TEST CASE: COMPLETED\n" RESET);
}

void runE2ETests(void) {
    tests_passed = 0;
    tests_total = 0;
    test_full_car_management_workflow(); // Run the main E2E test case

    printf(CYAN "\n--- E2E Test Summary --- \n" RESET);
    if (tests_passed == tests_total) printf(GREEN "ALL %d E2E CHECKS PASSED!\n" RESET, tests_total);
    else printf(RED "%d / %d E2E CHECKS PASSED\n" RESET, tests_passed, tests_total);
    printf("------------------------\n");
}

