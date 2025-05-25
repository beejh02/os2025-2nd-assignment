#include <iostream>
#include "queue.h"


Queue* init(void) {
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL) return NULL;
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}


void release(Queue* queue) {
	if (queue == NULL) return;
	Node* current = queue->head;

	while (current != NULL) {
		Node* temp = current;
		current = current->next;
		nfree(temp);
	}

	free(queue);
}


Node* nalloc(Item item) {
	Node* node = (Node*)malloc(sizeof(Node));

	if (node == NULL) return NULL;
	node->item = item;
	node->next = NULL;

	return node;
}


void nfree(Node* node) {
	if (node != NULL) {
		free(node);
	}
	return;
}


Node* nclone(Node* node) {
	return NULL;
}


Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, NULL };
	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
