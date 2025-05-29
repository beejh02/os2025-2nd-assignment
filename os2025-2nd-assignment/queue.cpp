#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "queue.h"


using namespace std;
mutex mtx;
condition_variable cv;


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
	if (node == NULL) return NULL;
	Node* new_node = nalloc(node->item);

	return new_node;
}


Reply enqueue(Queue* queue, Item item) {
	Reply reply = { false, { 0, NULL } };
	if (queue == NULL) return reply;

	Node* new_node = nalloc(item);
	if (new_node == NULL) return reply;

	{
		lock_guard<mutex> lock(mtx);
		reply.success = true;
		reply.item = item;

		// if first, point to the same thing(head, tail)
		if (queue->head == NULL) {
			queue->head = new_node;
			queue->tail = new_node;
			return reply;
		}

		// search, descending order
		Node* current = queue->head;
		Node* prev = NULL;

		while (current != NULL && current->item.key > item.key) {
			prev = current;
			current = current->next;
		}

		if (prev == NULL) {
			new_node->next = queue->head;
			queue->head = new_node;

			if (queue->tail == NULL) {
				queue->tail = new_node;
			}
		}
		else {
			new_node->next = current;
			prev->next = new_node;
			if (current == NULL) {
				queue->tail = new_node;
			}
		}
	}

	cv.notify_one();

	return reply;
}

Reply dequeue(Queue* queue) {
	Reply reply = { false, {0, NULL} };
	if (queue == NULL) return reply;

	unique_lock<mutex> lock(mtx);
	cv.wait(lock, [queue] {return queue->head != NULL; });

	Node* temp = queue->head;
	queue->head = queue->head->next;

	if (queue->head == NULL) {
		queue->tail = NULL;
	}

	reply.success = true;
	reply.item = temp->item;
	nfree(temp);

	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	
	// init() -> make a new queue
	Queue* new_queue = init();

	if (new_queue == NULL || queue == NULL) return new_queue;

	Node* current = queue->head;

	// start <= key && key <= end
	while (current != NULL) {
		if (start <= current->item.key  && current->item.key <= end) {
			Item item = current->item;
			enqueue(new_queue, item);
		}
		current = current->next;
	}

	return new_queue;
}
