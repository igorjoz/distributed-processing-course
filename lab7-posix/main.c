#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int START = 0;
const int STOP = 100000;
const int THREADS_NUMBER = 4;

void *printIfIsPrimeThread(int threadId) {
    int tid = (int)threadId;
    static int numbersCount=0;
    int start = START + (STOP - START) * numbersCount / THREADS_NUMBER;
    int stop = START + (STOP - START) * (numbersCount + 1) / THREADS_NUMBER;
    numbersCount += 1;
    int isPrime;

    while (start <= stop) {
        isPrime = 1;

        if (start < 2) {
             isPrime = 0;
        }

        for (int i = 2; i * i <= start && isPrime; i++) {
            if (start % i == 0) {
                isPrime = 0;
            }
        }

        if (isPrime == 1) {
            printf("%d ", start);
        }

        start++;
    }

    printf("\n\n");
}

void checkPrimesThread(int START, int STOP, int THREADS_NUMBER) {
    pthread_t threads[THREADS_NUMBER];
    int createdThread;
    
    for (int threadId = 0; threadId < THREADS_NUMBER; threadId++) {
        createdThread = pthread_create(&threads[threadId], NULL, printIfIsPrimeThread, threadId);

        if (createdThread != 0) {
            printf("return code: %d\n", createdThread);
            exit(1);
        }

        pthread_join(threads[threadId], NULL);
    }
}

int main()
{
    checkPrimesThread(START, STOP, THREADS_NUMBER);

    pthread_exit(NULL);

    return 0;
}