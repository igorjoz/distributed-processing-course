#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

#define ITEMS_COUNT 10
#define FILENAME "prod_cons_data.txt"

sem_t *emptySlots;
sem_t *filledSlots;
sem_t *mutex;

void producer() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ITEMS_COUNT; ++i) {
        sem_wait(emptySlots);
        sem_wait(mutex);

        fprintf(file, "%d\n", i);
        fflush(file); // Ensure the data is written to the file system
        printf("Producer produced item %d\n", i);

        sem_post(mutex);
        sem_post(filledSlots);

        sleep(1);
    }

    fclose(file);
}

void consumer() {
    // Wait for the producer to finish producing all items
    for (int i = 0; i < ITEMS_COUNT; ++i) {
        sem_wait(filledSlots);
    }

    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    int item;
    while (fscanf(file, "%d\n", &item) == 1) { // Read until EOF
        printf("Consumer consumed item %d\n", item);
        sem_post(emptySlots); // Signal capacity for more items
        sleep(2); // Simulate consumption delay
    }

    fclose(file); // Close the file
}
int main() {
    // Initialize semaphores
    emptySlots = sem_open("/emptySlots", O_CREAT | O_EXCL, 0644, ITEMS_COUNT);
    filledSlots = sem_open("/filledSlots", O_CREAT | O_EXCL, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0644, 1);

    unlink(FILENAME); // Ensure the file does not exist before starting

    pid_t pid = fork();
    if (pid == 0) {
        // Child process - Consumer
        consumer();
        exit(0);
    } else if (pid > 0) {
        // Parent process - Producer
        producer();
        wait(NULL); // Wait for the child process to finish
    }

    // Cleanup
    sem_close(emptySlots);
    sem_close(filledSlots);
    sem_close(mutex);
    sem_unlink("/emptySlots");
    sem_unlink("/filledSlots");
    sem_unlink("/mutex");

    return 0;
}
