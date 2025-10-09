#include <stdio.h>
#include <stdlib.h>
#include "car.h"
#include "fileio.h"
#include "tests.h"

int main(void) {
    initFileIfMissing(FILENAME);

    int choice;
    for (;;) {
        printf("\n===== Car Management System =====\n");
        printf("1. Add Car\n");
        printf("2. Display Cars\n");
        printf("3. Search Car (by Model or Year)\n");
        printf("4. Update Car\n");
        printf("5. Delete Car\n");
        printf("6. Run Unit Tests\n");    
        printf("7. Run E2E Test\n");     
        printf("8. Exit\n");
        printf("Choose an option: ");

        char buf[16];
        if (!fgets(buf, sizeof(buf), stdin)) {
            break; 
        }
        choice = atoi(buf);

        switch (choice) {
            case 1: addCarInteractive(FILENAME); break;
            case 2: displayCars(FILENAME); break;
            case 3: searchCarInteractive(FILENAME); break;
            case 4: updateCarInteractive(FILENAME); break;
            case 5: deleteCarInteractive(FILENAME); break;
            case 6: runUnitTests(); break;  
            case 7: runE2ETests(); break;   
            case 8: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
