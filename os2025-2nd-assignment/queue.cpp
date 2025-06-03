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
	while (curr != nullptr) {
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
	clone->next = nullptr;

	return clone;
}


Reply enqueue(Queue* queue, Item item) {

    Reply reply;
    reply.success = false;
    reply.item = item;

    Node* new_node = nalloc(item);


    while (1) {
        Node* prev = queue->head;
        Node* curr = prev->next;

        while (curr != nullptr && curr->item.key > item.key) {
            prev = curr;
            curr = curr->next;
        }

        if (curr != nullptr && curr->item.key == item.key) {
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
	Reply reply = { false, NULL };
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	return NULL;
}
