#include <stdio.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
    long podkanal;
    int number;
} message;

struct m_komunikat {
    long podkanal ;
    char linia1[200];
    int index;
} komunikat;

int main() {
    long channel = 0;
    int flags = 0666;
    // int msgget(long channel, int flags);
    int msgetResult = msgget(channel, flags);

    // wypisz identyfikator dostępu
    printf("%d\n", msgetResult);

    long subchannel = 111;
    message.number = 678;
    message.podkanal = subchannel;

    komunikat.podkanal = subchannel;
    komunikat.index = 123;

    // int wielk = 789;
    // int msgsndResult = msgsnd(channel, &message, wielk, flags);
    int msgsndResult = msgsnd(channel, (struct msgbuf*)&komunikat, sizeof(komunikat), flags);

    printf("%d\n", msgsndResult);


    int msgrcvResult = msgrcv(channel, (struct msgbuf*)&komunikat, sizeof(komunikat), subchannel, flags);

    printf("%d\n", msgrcvResult);

    // int msgrcv(int kanal, struct msgbuf* komunikat, int wielk, long podkanal, int flagi);

    // int msgctl(int kanal, int polecenie, struct msqid_ds* parametry);


    return 0;
}
