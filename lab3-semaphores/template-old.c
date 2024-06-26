#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <pqueue.h>

char *filename = "queue.dat";

int itemId = 0;

typedef struct item item;
struct item {
	int id;
};

item *
produce() {
	int time = rand() % 5 + 1;
	item *it = (item *)malloc(sizeof(item));
	
	sleep(time);
	it->id = itemId;
	itemId += 1;
	return it;
}

void
consume(item *it) {
	int time = rand() % 5 + 1;
	sleep(time);
	free(it);
}


void
producer(pid_t childPid) {
}

void consumer() {
	pqueue* qu = NULL;
	qunserialize(&qu, sizeof(item), filename);

	if (qu == NULL)
	{
		sleep(rand() % 5 + 1);
		printf("Consumer: Empty queue\n");
		return;
	}

	void* data = qpop(&qu)->data;
	printf("Consumed: %d\n", ((item*)data)->id);
	qserialize(qu, sizeof(item), filename);
	free(data);
	sleep(rand() % 5 + 1);
}

int main(int argc, char **argv) {
	pid_t pid;
	pqueue* qu = NULL;

	if ((pid = fork()) == 0) {
		for (;;) {
			producer(pid, qu);
		}
		return 0;
	}
	else if (pid > 0) {
		printf("Yo\n");
		for (;;) {
			consumer();
		}
	}
	else {
		perror("Fork failed\n");
		return 1;
	}

}