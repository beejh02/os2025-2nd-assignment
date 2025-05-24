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
	// nfree() 먼저 구현 필요
	// 아래 있음


	return;
}


Node* nalloc(Item item) {
	// Node 생성, item으로 초기화
	return NULL;
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
