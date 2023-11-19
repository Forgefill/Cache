#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define ARRAY_SIZE 10000000
#define ITERATIONS 600

double getTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void sequentialAccess(volatile int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = i;
    }
}

void randomAccess(volatile int *array, int *randomIndices, int size) {
    for (int i = 0; i < size; i++) {
        array[randomIndices[i]] = i;
    }
}

int main() {
    volatile int *array = malloc(ARRAY_SIZE * sizeof(int));
    int *randomIndices = malloc(ARRAY_SIZE * sizeof(int));

    if (!array || !randomIndices) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Precompute random indices
    srand(123); // Use a fixed seed for reproducibility
    for (int i = 0; i < ARRAY_SIZE; i++) {
        randomIndices[i] = rand() % ARRAY_SIZE;
    }

    double startTime, endTime;

    // Sequential access
    startTime = getTime();
    for (int i = 0; i < ITERATIONS; i++) {
        sequentialAccess(array, ARRAY_SIZE);
    }
    endTime = getTime();
    printf("Time taken for sequential access: %f seconds\n", endTime - startTime);

    // Random access (with precomputed indices)
    startTime = getTime();
    for (int i = 0; i < ITERATIONS; i++) {
        randomAccess(array, randomIndices, ARRAY_SIZE);
    }
    endTime = getTime();
    printf("Time taken for random access: %f seconds\n", endTime - startTime);

    free((void*)array);
    free(randomIndices);
    return 0;
}
