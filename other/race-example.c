#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUMBER 400000
int globalCounter = 0;
sem_t sem;

void *inc(void* vp) {
    int i = NUMBER;
    while(i--) {
        sem_wait(&sem);
        globalCounter++;
        sem_post(&sem);
    }
}


int main(void) {
    pthread_t t1, t2, t3;
    if(sem_init(&sem, 0, 1) != 0)
        return 1;

    printf("%d\n", globalCounter);
    pthread_create(&t1, NULL, inc, NULL);
    pthread_create(&t2, NULL, inc, NULL);
    pthread_create(&t3, NULL, inc, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    printf("%d\n", globalCounter);

    return 0;
}
