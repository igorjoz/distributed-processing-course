#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "pqueue.h"

#define ITEMS_COUNT 10
#define FILENAME "prod_cons_data.txt"

sem_t *emptySlots;
sem_t *filledSlots;
sem_t *mutex;

void producer() {
    pqueue *head = NULL;

    for (int i = 0; i < ITEMS_COUNT; ++i) {
        sem_wait(emptySlots);
        sem_wait(mutex);

        int* item = malloc(sizeof(int));
        *item = i;
        qinsert(&head, item, i);

        qserialize(head, sizeof(int), FILENAME);

        printf("Producer produced item %d\n", i);

        sem_post(mutex);
        sem_post(filledSlots);

        sleep(1);
    }

    // while (head != NULL) {
    //     pqueue *tmp = head;
    //     head = head->next;
    //     free(tmp->data);
    //     free(tmp);
    // }
}


void consumer() {
    pqueue *head = NULL;

    for (int i = 0; i < ITEMS_COUNT; ++i) {
        sem_wait(filledSlots);
        sem_wait(mutex);

        qunserialize(&head, sizeof(int), FILENAME);

        pqueue *itemNode = qpop(&head);
        if (itemNode != NULL) {
            int *item = itemNode->data;
            printf("Consumer consumed item %d\n", *item);
            free(item);
            free(itemNode);
        }

        qserialize(head, sizeof(int), FILENAME);

        sem_post(mutex);
        sem_post(emptySlots);

        sleep(2);
    }

    // while (head != NULL) {
    //     pqueue *tmp = head;
    //     head = head->next;
    //     free(tmp->data);
    //     free(tmp);
    // }
}


int main() {
    // sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
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
