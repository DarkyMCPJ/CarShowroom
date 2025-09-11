#include <stdio.h>

#define FILENAME "cars.csv"
#define MAX_MODEL 50

// Car structure for header idk
typedef struct {
    char model[MAX_MODEL];
    int year;
    float price;
    char availability[10];
} Car;

// Function and stuff
void addCar();
void displayCars();
void searchCar();
void updateCar();
void deleteCar();

int main() {

    for(int choice;;) {
        printf("\n===== Car Management System =====\n");
        printf("1. Add Car\n");
        printf("2. Display Cars\n");
        printf("3. Search Car (by Model or Year)\n");
        printf("4. Update Car\n");
        printf("5. Delete Car\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // for cleaner line

        switch (choice) {
            case 1: addCar(); break;
            case 2: displayCars(); break;
            case 3: searchCar(); break;
            case 4: updateCar(); break;
            case 5: deleteCar(); break;
            case 6: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    } 
}


// Add car to CSV
void addCar() {

}

// Display all cars
void displayCars() {

}

// Search car
void searchCar() {

}

// Update car
void updateCar() {

}

// Delete car
void deleteCar() {

}
