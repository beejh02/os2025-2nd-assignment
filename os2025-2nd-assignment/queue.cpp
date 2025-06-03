#include <iostream>
#include "queue.h"


Queue* init(void) {
	Queue* q = new Queue;
	Node* dummy = new Node;

	dummy->item.key = 0;
	dummy->item.value = NULL;
	dummy->next = NULL;

	q->head = dummy;
	return q;
}

void release(Queue* queue) {
	Node* curr = queue->head;
	while (curr != NULL) {
		Node* next = curr->next;
		delete curr;
		curr = next;
	}
	delete queue;
}

Node* nalloc(Item item) {
	Node* node = new Node;
	node->item = item;
	node->next = NULL;
	return node;
}


void nfree(Node* node) {
	delete node;
}


Node* nclone(Node* node) {
	if (!node) return NULL;

	Node* clone = new Node;
	clone->item = node->item;
	clone->next = NULL;

	return clone;
}


Reply enqueue(Queue* queue, Item item) {

    Reply reply = { false, { 0, NULL } };
    Node* new_node = nalloc(item);

    while (1) {
        Node* prev = queue->head;
        Node* curr = prev->next;

        while (curr != NULL && curr->item.key > item.key) {
            prev = curr;
            curr = curr->next;
        }

        if (curr != NULL && curr->item.key == item.key) {
            Node* next = curr->next;
            new_node->next = next;

            if (prev->next.compare_exchange_weak(curr, new_node)) {
                nfree(curr);
                reply.success = true;
                return reply;
            }
        }
        else {
            new_node->next = curr;
            if (prev->next.compare_exchange_weak(curr, new_node)) {
                reply.success = true;
                return reply;
            }
        }
    }
}


Reply dequeue(Queue* queue) {
    Reply reply = { false, { 0, NULL } };

    while (1) {
        Node* prev = queue->head;
        Node* first = prev->next;

        if (first == NULL) {
            return reply;
        }

        Node* next = first->next;
        if (prev->next.compare_exchange_weak(first, next)) {
            reply.success = true;
            reply.item = first->item;
            nfree(first);
            return reply;
        }
    }
}


Queue* range(Queue* queue, Key start, Key end) {
    Queue* new_q = init();

    Node* curr = queue->head.load()->next.load();

    while (curr != NULL) {
        Key k = curr->item.key;
        if (k >= start && k <= end) {
            enqueue(new_q, curr->item);
        }
        curr = curr->next;
    }

    return new_q;
}