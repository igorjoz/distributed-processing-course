#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pqueue.h"

sem_t emptySlots; // track producer availability to insert items
sem_t filledSlots; // track consumer availability to consume items
pthread_mutex_t mutex; // mutex for protecting critical sections involving queue operations

pqueue *queue = NULL;

void* producer(void* arg) {
    for (int i = 0; i < 10; ++i) {
        sem_wait(&emptySlots); // make sure there's capacity
        pthread_mutex_lock(&mutex); // critical section - start

        // Simulate item production
        int *item = malloc(sizeof(int));
        *item = i;
        qinsert(&queue, item, i);
        printf("Producer produced item %d\n", *item);

        pthread_mutex_unlock(&mutex); // critical section - end
        sem_post(&filledSlots); // signal an item is available

        sleep(1);
    }

    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; ++i) {
        sem_wait(&filledSlots); // wait for an item to be available
        pthread_mutex_lock(&mutex);

        pqueue *itemNode = qpop(&queue);

        if (itemNode) {
            int* item = (int*)itemNode->data;
            printf("Consumer consumed item %d\n", *item);
            free(item);
            free(itemNode);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&emptySlots); // signal capacity for more items

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t producerThread, consumerThread;

    sem_init(&emptySlots, 0, 10);
    sem_init(&filledSlots, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    // wait for threads to finish
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    sem_destroy(&emptySlots);
    sem_destroy(&filledSlots);
    pthread_mutex_destroy(&mutex);

    // while (queue) {
    //     pqueue *temp = queue;
    //     queue = queue->next;
    //     free(temp->data);
    //     free(temp);
    // }

    return 0;
}
