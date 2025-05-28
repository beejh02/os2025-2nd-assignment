#include <iostream>
#include <cstdlib>
#include <ctime>
#include "queue.h"


// <cstdlib> & <ctime> library import >> for real random


using namespace std;

int generate_random_key() {
    return (rand() % 500) + 1;
}

void print_queue(Queue* queue) {
    if (queue == NULL || queue->head == NULL) {
        cout << "empty queue" << endl;
        return;
    }

    Node* current = queue->head;
    cout << "Queue contents (key, value): \n\n";
    while (current != NULL) {
        cout << "(" << current->item.key << ", " << (int)current->item.value << ") ";
        current = current->next;
    }
    cout << endl;
}

int main(void) {
    srand((unsigned int)time(NULL));

    Queue* queue = init();
    if (queue == NULL) {
        cout << "Null queue" << endl;
        return 1;
    }

    // insert rand_number(1~500)
    const int NUM_ITEMS = 10;
    for (int i = 0; i < NUM_ITEMS; i++) {
        Item item;
        item.key = generate_random_key();
        item.value = (void*)(intptr_t)item.key;

        Reply reply = enqueue(queue, item);
        if (!reply.success) {
            cout << "success == fail(0)" << item.key << endl;
        }
    }

    print_queue(queue);

    // releasing queue
    release(queue);
    cout << "\nqueue released" << endl;

    return 0;
}