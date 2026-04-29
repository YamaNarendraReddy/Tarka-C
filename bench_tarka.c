#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// Mocking Tarka functions if compiled with standard CC
#ifndef TARKA_ENABLED
#define __tarka_check(ptr) 
#define __tarka_register(ptr, sz, heap)
#define __tarka_unregister(ptr)
#endif

void run_benchmark() {
    int size = 1000000;
    int *arr = malloc(size * sizeof(int));
    __tarka_register(arr, size * sizeof(int), 1);

    clock_t start = clock();
    
    volatile int sum = 0;
    for (int i = 0; i < size; i++) {
        // Every access here will be instrumented by Tarka-C
        sum += arr[i]; 
    }

    clock_t end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time for %d iterations: %f seconds\n", size, time_spent);
    
    __tarka_unregister(arr);
    free(arr);
}

int main() {
    run_benchmark();
    return 0;
}
