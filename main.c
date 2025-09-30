#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "cars.csv"
#define MAX_LINE 256
#define MAX_MODEL 50

// Car structure
typedef struct {
    char model[MAX_MODEL];
    int year;
    float price;
    char availability[10];
} Car;

// Function declarations
void addCar();
void displayCars();
void searchCar();
void updateCar();
void deleteCar();
void unitTest();
void E2ETest();
void initFile();

int main() {
initFile();
    for(int choice;;) {
        printf("\n===== Car Management System =====\n");
        printf("1. Add Car\n");
        printf("2. Display Cars\n");
        printf("3. Search Car (by Model or Year)\n");
        printf("4. Update Car\n");
        printf("5. Delete Car\n");
        printf("6. Unit Test\n");
        printf("7. E2E Test\n");
        printf("8. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1: addCar(); break;
            case 2: displayCars(); break;
            case 3: searchCar(); break;
            case 4: updateCar(); break;
            case 5: deleteCar(); break;
            case 6: unitTest(); break; 
            case 7: E2ETest(); break;
            case 8: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    } 
}

void initFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        fp = fopen(FILENAME, "w");
        if (fp) {
            fprintf(fp, "CarModel,Year,Price,Availability\n"); // ✅ CSV header only
            fclose(fp);
        }
    } else {
        fclose(fp);
    }
}

void addCar() {

}

void displayCars() {

}

void searchCar() {

}

void updateCar() {

}

void deleteCar() {

}

void unitTest() {
    
    int choice;
    for (;;) {
        printf("\n===== Unit Test Menu =====\n");
        printf("1. Test Add Car\n");
        printf("2. Test Display Cars\n");
        printf("3. Test Search Car\n");
        printf("4. Test Update Car\n");
        printf("5. Test Delete Car\n");
        printf("6. Back to Main Menu\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: 
                printf("\n--- Running Add Car Test ---\n");
                break;
            case 2: 
                printf("\n--- Running Display Cars Test ---\n");
                break;
            case 3: 
                printf("\n--- Running Search Car Test ---\n");
                break;
            case 4: 
                printf("\n--- Running Update Car Test ---\n");
                break;
            case 5: 
                printf("\n--- Running Delete Car Test ---\n");
                break;
            case 6: 
                return;
            default: 
                printf("Invalid choice!\n");
        }
    }
}

void E2ETest(){

}
