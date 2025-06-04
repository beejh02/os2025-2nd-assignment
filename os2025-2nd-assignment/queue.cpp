#include <iostream>
#include "queue.h"


Queue* init(void) {
	return NULL;
}


void release(Queue* queue) {
	return;
}


Node* nalloc(Item item) {
	Node* node = (Node*)malloc(sizeof(Node));

	if (node == NULL) {
		return NULL;
	}

	node->item = item;
	node->next = NULL;

	return node;
}


void nfree(Node* node) {
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
