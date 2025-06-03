#include <iostream>
#include "queue.h"

using namespace std;

static inline int parentIndex(int i) {
    return (i - 1) / 2;
}

static inline int leftChild(int i) {
    return 2 * i + 1;
}

static inline int rightChild(int i) {
    return 2 * i + 2;
}

static inline void swapItem(Item* a, Item* b) {
    Item tmp = *a;
    *a = *b;
    *b = tmp;
}


static void heapifyUp(Item* data, int idx) {
    while (idx > 0) {
        int p = parentIndex(idx);
        if (data[p].key < data[idx].key) {
            swapItem(&data[p], &data[idx]);
            idx = p;
        }
        else {
            break;
        }
    }
}


static void heapifyDown(Item* data, int sz, int idx) {
    while (true) {
        int left = leftChild(idx);
        int right = rightChild(idx);
        int largest = idx;

        if (left < sz && data[left].key > data[largest].key) {
            largest = left;
        }

        if (right < sz && data[right].key > data[largest].key) {
            largest = right;
        }
        if (largest != idx) {
            swapItem(&data[idx], &data[largest]);
            idx = largest;
        }
        else {
            break;
        }
    }
}


Queue* init(void) {
    Queue* queue = new Queue;
    if (queue == NULL) return NULL;

    queue->size = 0;
    return queue;
}


void release(Queue* queue) {
    if (queue == NULL) return;
    delete queue;
}


Reply enqueue(Queue* queue, Item item) {
    Reply reply = { false, { 0, NULL } };
    if (queue == NULL) return reply;

    {
        lock_guard<mutex> lock(queue->mtx);

        if (queue->size >= MAX_CAPACITY) {
            return reply;
        }

        int idx = queue->size;
        queue->data[idx] = item;
        queue->size += 1;

        heapifyUp(queue->data, idx);

        reply.success = true;
        reply.item = item;
    }
    return reply;
}


Reply dequeue(Queue* queue) {
    Reply reply = { false, { 0, NULL } };
    if (queue == NULL) return reply;

    lock_guard<std::mutex> lock(queue->mtx);

    if (queue->size <= 0) {
        return reply;
    }

    Item topItem = queue->data[0];

    queue->data[0] = queue->data[queue->size - 1];
    queue->size -= 1;

    if (queue->size > 0) {
        heapifyDown(queue->data, queue->size, 0);
    }

    reply.success = true;
    reply.item = topItem;
    return reply;
}


Node* nalloc(Item item) {
    return NULL;
}


void nfree(Node* node) {
    (void)node;
}


Node* nclone(Node* node) {
    (void)node;
    return NULL;
}


Queue* range(Queue* queue, Key start, Key end) {
    if (queue == NULL) return NULL;

    Queue* new_queue = init();
    if (new_queue == NULL) return NULL;

    {
        std::lock_guard<std::mutex> lock(queue->mtx);
        for (int i = 0; i < queue->size; ++i) {
            Item it = queue->data[i];
            if (start <= it.key && it.key <= end) {
                Item deepcopy;
                deepcopy.key = it.key;
                deepcopy.value_size = it.value_size;

                deepcopy.value = malloc(it.value_size);
                if (deepcopy.value != NULL) {
                    memcpy(deepcopy.value, it.value, it.value_size);
                }
                else {
                    deepcopy.value = NULL;
                }

                enqueue(new_queue, deepcopy);
            }
        }
    }

    return new_queue;
}