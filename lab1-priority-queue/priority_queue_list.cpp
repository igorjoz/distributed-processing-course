#include <stdio.h>
#include <stdlib.h>
#include "priority_queue_list.h"

void printList(pqueue* head, void (*print_data)(void*)) {
	pqueue* p;
    int counter = 0;

	for (p = head; p != NULL; p = p->next) {
        printf("Element %d: ", counter);
        printf("\tValue %d: ", p->k);
        printf("\t");
		print_data(p->data);
		printf("\n");

        counter++;
	}

    printf("\n");
}

void qinsert(pqueue** phead, void* data, int k) {
    // Allocate memory for a new node, assign values to it
    pqueue* new_node = (pqueue*)malloc(sizeof(pqueue));
    new_node->data = data;
    new_node->k = k;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Descending
    //if (*phead == NULL || (*phead)->k < k) {

    // Ascending
    if (*phead == NULL || (*phead)->k > k) { // When list is empty or it should be first element
        new_node->next = *phead;

        if (*phead != NULL) {
            (*phead)->prev = new_node;
        }

        *phead = new_node;
    }
    else {
        pqueue* current = *phead;

        // Descending
        //while (current->next != NULL && current->next->k >= k) {

        // Ascending
        while (current->next != NULL && current->next->k <= k) { // Search for proper place for the element
            current = current->next;
        }

        new_node->next = current->next;

        if (current->next != NULL) {
            current->next->prev = new_node;
        }

        current->next = new_node;
        new_node->prev = current;
    }
}

void qremove(pqueue** phead, int k) {
    // Empty list
    if (*phead == NULL) {
        return;
    }

    pqueue* temp = *phead;

    // If first element is the element we are looking for
    if (temp->k == k) {
        *phead = temp->next;

        if (*phead != NULL) {
            (*phead)->prev = NULL;
        }

        free(temp);

        return;
    }

    // Iterate list as long as element is not found
    while (temp != NULL && temp->k != k) {
        temp = temp->next;
    }

    // Key not found
    if (temp == NULL) {
        return;
    }

    // Rearrange pointers to previous & next elements
    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    }

    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    }

    // Delete found node
    free(temp);
}
