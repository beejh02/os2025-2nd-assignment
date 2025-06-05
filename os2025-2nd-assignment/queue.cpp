#include <iostream>
#include "queue.h"
#include <atomic>


using namespace std;

Queue* init(void) {
}

void release(Queue* queue) {
}

Node* nalloc(Item item) {
}

void nfree(Node* node) {
}

Node* nclone(Node* node) {
    return;
}

Reply enqueue(Queue* queue, Item item) {
    Reply reply = { false, {0, NULL, 0} };
    return reply;
}
Reply enqueue_nonatomic(Queue* queue, Item item) {
    Reply reply = { false, {0, NULL, 0} };
    return reply;
}

Reply dequeue(Queue* queue) {
    Reply reply = { false, {0, NULL, 0} };
    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    return queue;
}