// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <termios.h>  
#include "car.h"
#include "fileio.h"
#include "tests.h"
#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED   "\033[31m"


char get_single_char() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt); 
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); 
    ch = getchar(); 
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    return ch;
}

int main(void) {
    initFileIfMissing(FILENAME);
    int choice;

    for (;;) {
        printf("\n===== Car Management System =====\n");
        printf("1. Add Car\n");
        printf("2. Display Cars\n");
        printf("3. Search Car\n");
        printf("4. Update Car\n");
        printf("5. Delete Car\n");
        printf("---------- TESTING SUITES ----------\n");
        printf("6. Enter Test Arena (Sandbox Mode)\n");
        printf("7. Run Automated Unit Tests\n");
        printf("8. Run Full E2E Test\n");
        printf("------------------------------------\n");
        printf("9. Exit\n");
        printf("Choose an option: ");

        char c = get_single_char();
        printf("%c\n", c); 
        

        choice = c - '0';

        switch (choice) {
            case 1: addCarInteractive(FILENAME); break;
            case 2: displayCars(FILENAME); break;
            case 3: searchCarInteractive(FILENAME); break;
            case 4: updateCarInteractive(FILENAME); break;
            case 5: deleteCarInteractive(FILENAME); break;
            case 6: runTestArena(); break;
            case 7: runUnitTests(); break;
            case 8: runE2ETests(); break;
            case 9: printf("Exiting...\n"); return 0;
            default:
                printf(RED "Invalid choice!\n" RESET);
                break;
        }

        printf("\nPress Enter to continue...");
        int key;
        while ((key = getchar()) != '\n' && key != EOF);
    }
}
