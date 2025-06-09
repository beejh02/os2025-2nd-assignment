#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <unordered_set>
#include "queue.h"

using namespace std;

Queue* q;
mutex print_mtx;
atomic<int> enqueue_count{ 0 };  // enqueue 성공 카운터
atomic<int> dequeue_count{ 0 };  // dequeue 성공 카운터

void print_item(const Item& item) {
    cout << "(" << item.key << ", ";
    if (item.value && item.value_size > 0) {
        cout.write(static_cast<char*>(item.value), item.value_size);
    }
    else {
        cout << "null";
    }
    cout << ")";
}

void print_queue() {
    Node* node = q->head->next[0];
    cout << "curQueue : ";
    while (node) {
        print_item(node->item);
        if (node->next[0]) cout << ", ";
        node = node->next[0];
    }
    cout << "\n";
}

void enqueue_thread() {
    unordered_set<Key> seen;
    for (int i = 0; i < 80; ++i) {
        int key = rand() % 1234;
        bool is_duplicate = !seen.insert(key).second;

        string value = "val" + to_string(i);
        int sz = (int)value.size();
        char* val_ptr = (char*)malloc(sz + 1);
        memcpy(val_ptr, value.c_str(), sz);
        val_ptr[sz] = '\0';

        Item item = { (Key)key, val_ptr, sz };
        Reply rep = enqueue(q, item);

        // 신규 키에 대해서만 카운트 증가
        if (rep.success && !is_duplicate) {
            ++enqueue_count;
        }

        {
            lock_guard<mutex> lock(print_mtx);
            cout << "[Thread 1] enqueue : ";
            print_item(item);
            cout << (rep.success ? " (ok)" : " (failed)") << "\n";
            print_queue();
        }

        free(val_ptr);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void dequeue_thread() {
    for (int i = 0; i < 80; ++i) {
        this_thread::sleep_for(chrono::milliseconds(80));

        Reply rep = dequeue(q);
        if (rep.success) {
            ++dequeue_count;
        }

        {
            lock_guard<mutex> lock(print_mtx);
            cout << "[Thread 2] dequeue : ";
            if (rep.success) {
                print_item(rep.item);
                free(rep.item.value);
            }
            else {
                cout << "(failed)";
            }
            cout << "\n";
            print_queue();
        }
    }
}

int main() {
    srand((unsigned)time(nullptr));
    q = init();

    thread t1(enqueue_thread);
    thread t2(dequeue_thread);
    t1.join();
    t2.join();

    // enqueue/dequeue 횟수와 현재 노드 수 비교
    int node_count = 0;
    for (Node* node = q->head->next[0]; node; node = node->next[0]) {
        ++node_count;
    }
    cout << "[Check] enqueue: " << enqueue_count.load()
        << ", dequeue: " << dequeue_count.load()
        << ", remaining nodes: " << node_count << "\n";

    release(q);
    return 0;
}
