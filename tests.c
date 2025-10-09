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

// Forward declarations to fix implicit declaration errors inside this file
void test_validation_normal_case();
void test_validation_boundary_cases();
void test_validation_extreme_cases();
void test_save_and_load_normal_case();
void test_load_from_empty_file();
void test_load_from_nonexistent_file();

// Globals & Helpers for E2E Test Assertions
static int tests_passed;
static int tests_total;

void assert_equal_int(int expected, int actual, const char* message) {
    tests_total++;
    if (expected == actual) {
        printf(GREEN "  ✓ PASS: " RESET "%s\n", message);
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
    if (fabs(expected - actual) < 0.001) {
        printf(GREEN "  ✓ PASS: " RESET "%s\n", message);
        tests_passed++;
    } else {
        printf(RED "  ✗ FAIL: " RESET "%s (Expected: %.2f, Got: %.2f)\n", message, expected, actual);
    }
}


// Manual Interactive Unit Test 

void runTestArena() {
    const char* source_file = "testdata.csv";
    const char* arena_file = "test_arena.csv";
    copyFile(source_file, arena_file);

    printf(CYAN "\n============================================\n" RESET);
    printf(CYAN "      Welcome to the Test Arena! 🏟️\n" RESET);
    printf("A temporary copy of the data has been created.\n");
    printf("You can now Add, Search, Update, or Delete freely.\n");
    printf("All changes will be discarded when you exit.\n");
    printf(CYAN "============================================\n" RESET);

    int choice;
    for (;;) {
        printf("\n--- Test Arena Menu ---\n");
        printf("1. Add Car\n");
        printf("2. Display Cars\n");
        printf("3. Search Car\n");
        printf("4. Update Car\n");
        printf("5. Delete Car\n");
        printf("0. Exit Test Arena\n");
        printf("-----------------------\n");
        printf("Your choice: ");

        char buf[16];
        if (!fgets(buf, sizeof(buf), stdin)) break;
        choice = atoi(buf);

        switch (choice) {
            case 1: addCarInteractive(arena_file); break;
            case 2: displayCars(arena_file); break;
            case 3: searchCarInteractive(arena_file); break;
            case 4: updateCarInteractive(arena_file); break;
            case 5: deleteCarInteractive(arena_file); break;
            case 0:
                remove(arena_file);
                printf(CYAN "\nExiting Test Arena. All temporary changes have been deleted.\n" RESET);
                return;
            default:
                printf(RED "\nInvalid choice. Please try again.\n" RESET);
        }
        printf("\nPress Enter to continue...");
        getchar();
    }
}


// Automated Unit Test Cases

// 1. Normal Testing: Test with perfectly valid data.
void test_validation_normal_case() {
    // Arrange: Create a car with standard, valid data.
    Car valid_car = {"Toyota Camry", 2022, 25000.00, "Yes"};

    // Act: Check if the data is valid.
    int result = isCarDataValid(&valid_car);

    // Assert: The result should be 1 (true).
    assert(result == 1);
    
    printf(GREEN "  ✓ test_validation_normal_case PASSED\n" RESET);
}


// 2. Boundary Testing: Test the edges of the valid year range.
void test_validation_boundary_cases() {
    // Arrange: Create cars with data at the very edge of the rules.
    Car car_lower_bound = {"Oldsmobile", 1886, 500.00, "No"};  // Earliest valid year
    Car car_upper_bound = {"Future Car", 2025, 90000.00, "Yes"}; // Latest valid year
    Car car_lower_invalid = {"Too Old", 1885, 500.00, "No"};     // Just below valid year
    Car car_upper_invalid = {"Too New", 2026, 90000.00, "Yes"};    // Just above valid year
    Car car_price_zero = {"Free Car", 2000, 0.00, "Yes"};      // Price of zero is valid

    // Act & Assert: Check each case.
    assert(isCarDataValid(&car_lower_bound) == 1);   // Should be valid
    assert(isCarDataValid(&car_upper_bound) == 1);   // Should be valid
    assert(isCarDataValid(&car_lower_invalid) == 0); // Should be invalid
    assert(isCarDataValid(&car_upper_invalid) == 0); // Should be invalid
    assert(isCarDataValid(&car_price_zero) == 1);    // Should be valid
    
    printf(GREEN "  ✓ test_validation_boundary_cases PASSED\n" RESET);
}


// 3. Extreme/Error Testing: Test with unexpected or error-inducing data.
void test_validation_extreme_cases() {
    // Arrange: Create cars with clearly invalid data.
    Car car_negative_price = {"Charity Car", 2020, -100.00, "Yes"};
    Car car_empty_model = {"", 2021, 20000.00, "Yes"};
    Car car_bad_avail = {"Toyota Camry", 2022, 25000.00, "Maybe"};

    // Act & Assert: Check each case.
    assert(isCarDataValid(&car_negative_price) == 0); // Should be invalid
    assert(isCarDataValid(&car_empty_model) == 0);    // Should be invalid
    assert(isCarDataValid(&car_bad_avail) == 0);       // Should be invalid
    
    printf(GREEN "  ✓ test_validation_extreme_cases PASSED\n" RESET);
}

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
    printf(BLUE "\n=== Running All Automated Unit Tests ===" RESET "\n");
    printf("\n--- Testing Validation Logic ---\n");
    test_validation_normal_case();
    test_validation_boundary_cases();
    test_validation_extreme_cases();
    printf("\n--- Testing File I/O Logic ---\n");
    test_save_and_load_normal_case();
    test_load_from_empty_file();
    test_load_from_nonexistent_file();
    printf(CYAN "\n--- All Automated Unit Tests Passed! --- \n" RESET);
    printf("----------------------------------------\n");
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

