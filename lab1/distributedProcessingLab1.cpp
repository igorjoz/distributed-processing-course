#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priority_queue_list.h"

typedef struct process process;
struct process {
	char name[40];
};

void print_process(void* data) {
	process* pr = (process*)data;
	printf("%s", pr->name);
}

int main(int argc, char** argv) {
	// Create a priority queue & 6 nodes
	pqueue* queue = NULL;
	process* p1, * p2, * p3, * p4, * p5, * p6;

	p1 = (process*)malloc(sizeof(process));
	strcpy_s(p1->name, "Firefox");
	p2 = (process*)malloc(sizeof(process));
	strcpy_s(p2->name, "Geany IDE");
	p3 = (process*)malloc(sizeof(process));
	strcpy_s(p3->name, "Important Calculations");
	p4 = (process*)malloc(sizeof(process));
	strcpy_s(p4->name, "Foo");
	p5 = (process*)malloc(sizeof(process));
	strcpy_s(p5->name, "Bar");
	p6 = (process*)malloc(sizeof(process));
	strcpy_s(p6->name, "Baz");

	// Print empty list
	printf("--Empty list--\n");
	printList(queue, print_process);

	// Insert elements into the queue
	qinsert(&queue, p1, 5);
	qinsert(&queue, p2, 2);
	qinsert(&queue, p3, 10);
	qinsert(&queue, p4, 10);
	qinsert(&queue, p5, 20);
	qinsert(&queue, p6, 30);

	// Insertions & deletions on list
	printf("List after inserting 6 elements:\n");
	printList(queue, print_process);

	qremove(&queue, 2);

	printf("List after removing element of value 2:\n");
	printList(queue, print_process);

	qinsert(&queue, p2, 11);

	printf("List after inserting element GeanyIDE of new value 11:\n");
	printList(queue, print_process);

	qremove(&queue, 5);

	printf("List after removing element of value 5:\n");
	printList(queue, print_process);

	qremove(&queue, 11);
	qremove(&queue, 10);
	qremove(&queue, 10);
	
	printf("List after removing 2 elements of value 10 & an element of value 11:\n");
	printList(queue, print_process);

	qremove(&queue, 20);
	qremove(&queue, 30);

	printf("--List after removing elements of value 20 & 30--\n");
	printList(queue, print_process);

	// Free allocated memory
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	free(p5);
	free(p6);

	return 0;
}
