#include <iostream>
#include "queue.h"

using namespace std;

int main(void) {
    cout << "test range function" << endl;

    Queue* queue = init();
    if (queue == NULL) {
        cout << "queue init erorr" << endl;
        return 1;
    }

    Item items[] = {
        { 50, (void*)50 },
        { 30, (void*)30 },
        { 70, (void*)70 },
        { 20, (void*)20 },
        { 40, (void*)40 }
    };
    for (int i = 0; i < 5; i++) {
        Reply reply = enqueue(queue, items[i]);
        if (!reply.success) {
            cout << "Enqueue failed for key " << items[i].key << "!" << endl;
            release(queue);
            return 1;
        }
    }

    // 30 <= key <= 50 range test
    Key start = 30;
    Key end = 50;
    Queue* result_queue = range(queue, start, end);

    if (result_queue == NULL) {
        cout << "range function failed to create new queue!" << endl;
        release(queue);
        return 1;
    }

    // test
    bool test_passed = true;
    int expected_keys[] = { 50, 40, 30 };
    int expected_count = 3;
    int actual_count = 0;

    Node* current = result_queue->head;

    while (current != NULL) {
        actual_count++;
        if (actual_count > expected_count || current->item.key != expected_keys[actual_count - 1]) {
            cout <<  actual_count << ": expected=" << expected_keys[actual_count - 1] << ", value=" << current->item.key << endl;
            test_passed = false;
        }
        if (current->item.value != (void*)(current->item.key)) {
            cout <<  current->item.key << ": expected=" << current->item.key << ", value=" << (int)current->item.value << endl;
            test_passed = false;
        }
        current = current->next;
    }

    if (actual_count != expected_count) {
        cout << expected_count << actual_count << endl;
        test_passed = false;
    }

    // original queue's integrity test
    int original_count = 0;
    current = queue->head;

    while (current != NULL) {
        original_count++;
        current = current->next;
    }
    if (original_count != 5) {
        cout << original_count << endl;
        test_passed = false;
    }

    cout << (test_passed ? "range test good" : "range test failed") << endl;

    release(queue);
    release(result_queue);

    return 0;
}