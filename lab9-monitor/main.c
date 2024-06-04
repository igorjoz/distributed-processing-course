#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf {
    long podkanal;
    int number;
};

struct m_komunikat {
    long podkanal;
    char linia1[200];
    int index;
};

void init_semaphore(int *semid, long channel, int flags) {
    *semid = msgget(channel, IPC_CREAT | flags);
    if (*semid == -1) {
        perror("msgget");
        exit(1);
    }

    struct msgbuf msg;
    msg.podkanal = 1;
    msg.number = 1; // semaphore initialized to 1 (available)

    if (msgsnd(*semid, &msg, sizeof(int), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
}

// wait / decrement
void P(int semid, int flags) {
    struct msgbuf msg;

    if (msgrcv(semid, &msg, sizeof(int), 1, flags) == -1) {
        perror("msgrcv");
        exit(1);
    }
}

// signal / increment
void V(int semid, int flags) {
    struct msgbuf msg;
    msg.podkanal = 1;
    msg.number = 1; // release semaphore

    if (msgsnd(semid, &msg, sizeof(int), flags) == -1) {
        perror("msgsnd");
        exit(1);
    }
}

int main() {
    int semid;
    long channel = 1234;
    int flags = 0666;

    init_semaphore(&semid, channel, flags);

    printf("Semaphore ID: %d\n", semid);

    printf("Trying to perform P (wait) operation...\n");
    P(semid, 0);
    printf("P operation successful! Semaphore acquired.\n");

    printf("In the critical section...\n");

    // release
    printf("Performing V (signal) operation...\n");
    V(semid, 0);
    printf("V operation successful! Semaphore released.\n");

    // send a message
    struct m_komunikat komunikat;
    komunikat.podkanal = 2;
    strcpy(komunikat.linia1, "Test message");
    komunikat.index = 123;

    if (msgsnd(semid, (struct msgbuf*)&komunikat, sizeof(komunikat) - sizeof(long), flags) == -1) {
        perror("msgsnd");
        exit(1);
    }
    printf("Message sent: %s\n", komunikat.linia1);

    // receive a message
    struct m_komunikat rcv_komunikat;
    long subchannel = 2;
    int msgrcvResult = msgrcv(semid, (struct msgbuf*)&rcv_komunikat, sizeof(rcv_komunikat) - sizeof(long), subchannel, flags);
    if (msgrcvResult == -1) {
        perror("msgrcv");
        exit(1);
    }
    printf("Message received: %s\n", rcv_komunikat.linia1);

    msgctl(semid, IPC_RMID, NULL);

    return 0;
}
