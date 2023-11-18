#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define ARRAY_SIZE 10000000  // Increased array size
#define STEP_SIZE  500
#define ITERATIONS 10        // Number of iterations

double getTime() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

void sequentialAccess(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = i;
    }
}

void randomAccess(int *array, int size) {
    for (int i = 0; i < size; i += STEP_SIZE) {
        array[rand() % size] = i;
    }
}

int main() {
    int *array = malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    double startTime, endTime;

    // Sequential access
    startTime = getTime();
    for (int i = 0; i < ITERATIONS; i++) {
        sequentialAccess(array, ARRAY_SIZE);
    }
    endTime = getTime();
    printf("Time taken for sequential access: %f seconds\n", endTime - startTime);

    // Random access
    startTime = getTime();
    for (int i = 0; i < ITERATIONS; i++) {
        randomAccess(array, ARRAY_SIZE);
    }
    endTime = getTime();
    printf("Time taken for random access: %f seconds\n", endTime - startTime);

    free(array);
    return 0;
}
