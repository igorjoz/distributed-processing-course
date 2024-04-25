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
        fflush(file);
        printf("Producer produced item %d\n", i);

        sem_post(mutex);
        sem_post(filledSlots);

        sleep(1);
    }

    fclose(file);
}

void consumer() {
    int item;
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ITEMS_COUNT; ++i) {
        sem_wait(filledSlots);
        sem_wait(mutex);

        clearerr(file);
        while (fscanf(file, "%d\n", &item) != 1) {
            clearerr(file);
            usleep(100000);
        }
        printf("Consumer consumed item %d\n", item);

        sem_post(mutex);
        sem_post(emptySlots);

        sleep(2);
    }

    fclose(file);
}

int main() {
    emptySlots = sem_open("/emptySlots", O_CREAT | O_EXCL, 0644, ITEMS_COUNT);
    filledSlots = sem_open("/filledSlots", O_CREAT | O_EXCL, 0644, 0);
    mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0644, 1);

    unlink(FILENAME);

    pid_t pid = fork();
    if (pid == 0) {
        consumer();
        exit(0);
    } else if (pid > 0) {
        producer();
        wait(NULL);
    }

    sem_close(emptySlots);
    sem_close(filledSlots);
    sem_close(mutex);
    sem_unlink("/emptySlots");
    sem_unlink("/filledSlots");
    sem_unlink("/mutex");

    return 0;
}
