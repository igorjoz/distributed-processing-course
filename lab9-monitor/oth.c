#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure representing the binary semaphore monitor
typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} binary_semaphore_t;

// Initialize the binary semaphore
void init_semaphore(binary_semaphore_t *sem, int initial_value) {
    sem->value = initial_value;
    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->cond, NULL);
}

// Perform P (wait) operation
void P(binary_semaphore_t *sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->value == 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->value = 0;
    pthread_mutex_unlock(&sem->mutex);
}

// Perform V (signal) operation
void V(binary_semaphore_t *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->value = 1;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

// Example critical section
void* critical_section(void* arg) {
    binary_semaphore_t *sem = (binary_semaphore_t*)arg;

    printf("Trying to perform P (wait) operation...\n");
    P(sem);
    printf("P operation successful! Semaphore acquired.\n");

    // Critical section
    printf("In the critical section...\n");
    sleep(1); // Simulate work

    printf("Performing V (signal) operation...\n");
    V(sem);
    printf("V operation successful! Semaphore released.\n");

    return NULL;
}

int main() {
    binary_semaphore_t sem;
    init_semaphore(&sem, 1);

    pthread_t t1, t2;

    // Create threads that will enter the critical section
    pthread_create(&t1, NULL, critical_section, (void*)&sem);
    pthread_create(&t2, NULL, critical_section, (void*)&sem);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    // Clean up
    pthread_mutex_destroy(&sem.mutex);
    pthread_cond_destroy(&sem.cond);

    return 0;
}
