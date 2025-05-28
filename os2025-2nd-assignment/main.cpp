#include <iostream>
#include "queue.h"

using namespace std;

int main(void) {
    cout << "testing nclone function" << endl;

    Item test_item = { 123, (void*)12345 };
    Node* original_node = nalloc(test_item);

    if (original_node == NULL) {
        cout << "nalloc failed(original node)" << endl;
        return 1;
    }

    Node* cloned_node = nclone(original_node);
    if (cloned_node == NULL) {
        cout << "nclone failed" << endl;
        nfree(original_node);
        return 1;
    }

    bool test_passed = true;
    if (cloned_node->item.key != original_node->item.key) {
        cout << original_node->item.key << ", cloned=" << cloned_node->item.key << endl;
        test_passed = false;
    }
    if (cloned_node->item.value != original_node->item.value) {
        cout << (int)original_node->item.value << ", cloned=" << (int)cloned_node->item.value << endl;
        test_passed = false;
    }
    if (cloned_node->next != NULL) {
        cout << "next pointer Null erorr" << endl;
        test_passed = false;
    }

    cout << (test_passed ? "nclone test good" : "nclone test failed") << endl;

    nfree(original_node);
    nfree(cloned_node);

    return 0;
}