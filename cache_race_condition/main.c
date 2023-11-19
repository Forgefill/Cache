#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 100
#define NUM_INCREMENTS 100000

atomic_int atomic_array[ARRAY_SIZE];
int array[ARRAY_SIZE] = {0};
pthread_mutex_t lock;

void *increment_array_with_race_condition() {
    for (int i = 0; i < NUM_INCREMENTS; ++i) {
        array[i % ARRAY_SIZE]++;
    }
    return NULL;
}

void *increment_array_with_mutex() {
    for (int i = 0; i < NUM_INCREMENTS; ++i) {
        pthread_mutex_lock(&lock);
        array[i % ARRAY_SIZE]++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *increment_array_with_atomic() {
    for (int i = 0; i < NUM_INCREMENTS; ++i) {
        atomic_fetch_add_explicit(&atomic_array[i % ARRAY_SIZE], 1, memory_order_relaxed);
    }
    return NULL;
}

void run_threads(void *(*function)(void *)) {
    pthread_t threads[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, function, NULL);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    // Initialize mutex
    pthread_mutex_init(&lock, NULL);

    // Run with race condition
    printf("Running with race condition...\n");
    run_threads(increment_array_with_race_condition);
    int sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }
    printf("Sum with race condition: %d\n", sum);

    // Run with mutex
    printf("Running with mutex...\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 0; // Reset array
    }
    run_threads(increment_array_with_mutex);
    sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += array[i];
    }
    printf("Sum with mutex: %d\n", sum);

    // Run with atomic operations
    printf("Running with atomic operations...\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        atomic_store(&atomic_array[i], 0); // Reset atomic array
    }
    run_threads(increment_array_with_atomic);
    sum = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        sum += atomic_load(&atomic_array[i]);
    }
    printf("Sum with atomic operations: %d\n", sum);

    // Destroy mutex
    pthread_mutex_destroy(&lock);

    return 0;
}
