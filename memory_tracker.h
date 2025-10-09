#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <stdio.h>
#include <stdlib.h>

static int g_alloc_counter = 0;


#define malloc(size)       my_malloc(size, __FILE__, __LINE__)
#define free(ptr)          my_free(ptr, __FILE__, __LINE__)


void* my_malloc(size_t size, const char* file, int line) {

    #undef malloc
    
    void* ptr = malloc(size); 
    
    #define malloc(size) my_malloc(size, __FILE__, __LINE__)

    if (ptr != NULL) {
        g_alloc_counter++;
    }
    return ptr;
}


void my_free(void* ptr, const char* file, int line) {

    #undef free

    if (ptr != NULL) {
        g_alloc_counter--;
    }
    free(ptr); 


    #define free(ptr) my_free(ptr, __FILE__, __LINE__)
}

// prints the final report
void report_memory_leaks() {
    printf("\n--- Memory Leak Report ---\n");
    if (g_alloc_counter == 0) {
        printf("✅ No memory leaks detected!\n");
    } else {
        printf("❌ WARNING: %d memory blocks were not freed!\n", g_alloc_counter);
    }
    printf("--------------------------\n");
}

#endif
