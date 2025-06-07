#include <iostream>
#include <cstring>
#include "queue.h"

// Test enqueue and dequeue operations: should output keys in descending order
void testEnqueueDequeue() {
    Queue* q = init();
    if (!q) {
        std::cerr << "Failed to initialize queue" << std::endl;
        return;
    }

    unsigned int keys[] = { 5, 1, 3, 4, 2 };
    for (int i = 0; i < 5; ++i) {
        Item it;
        it.key = keys[i];
        it.value = nullptr;
        it.value_size = 0;
        Reply rep = enqueue(q, it);
        std::cout << "Enqueue key=" << it.key << (rep.success ? " success" : " failure") << std::endl;
    }

    std::cout << "\nDequeue keys (expected 5 4 3 2 1):\n";
    Reply r;
    while ((r = dequeue(q)).success) {
        std::cout << r.item.key << " ";
    }
    std::cout << std::endl;

    release(q);
}

// Test range operation: should output keys between start and end in descending order
void testRange() {
    Queue* q = init();
    if (!q) {
        std::cerr << "Failed to initialize queue" << std::endl;
        return;
    }

    unsigned int keys[] = { 5, 1, 3, 4, 2 };
    for (int i = 0; i < 5; ++i) {
        Item it;
        it.key = keys[i];
        it.value = nullptr;
        it.value_size = 0;
        enqueue(q, it);
    }

    Key start = 2;
    Key end = 4;
    Queue* rq = range(q, start, end);
    if (!rq) {
        std::cerr << "Range query failed" << std::endl;
        release(q);
        return;
    }

    std::cout << "\nRange [" << start << "," << end << "] dequeue keys (expected 4 3 2):\n";
    Reply r;
    while ((r = dequeue(rq)).success) {
        std::cout << r.item.key << " ";
    }
    std::cout << std::endl;

    release(rq);
    release(q);
}

int main() {
    std::cout << "=== Testing enqueue/dequeue ===" << std::endl;
    testEnqueueDequeue();

    std::cout << "=== Testing range ===" << std::endl;
    testRange();

    return 0;
}
