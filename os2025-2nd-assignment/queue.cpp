#include <iostream>
#include "queue.h"


Queue* init(void) {
	Queue* q = new Queue;
	Node* dummy = new Node;

	dummy->item.key = 0;
	dummy->item.value = NULL;
	dummy->next = (NULL);

	q->head = (dummy);
	return q;
}

void release(Queue* queue) {
	return;
}


Node* nalloc(Item item) {
	// Node 생성, item으로 초기화
	return NULL;
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
