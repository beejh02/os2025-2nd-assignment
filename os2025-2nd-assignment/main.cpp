#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include "queue.h"     // init, enqueue, dequeue, release 선언
#include "qtype.h"     // Queue, Node, Item 정의

using namespace std;

// cout 동기화를 위한 mutex
mutex print_mtx;

// 바닥 레벨(0)을 순회하며 (key,value) 형식으로 출력
void printQueue(Queue* q) {
    lock_guard<mutex> lock(q->mtx);
    lock_guard<mutex> cout_lock(print_mtx);
    cout << "current queue : ";
    Node* cur = q->head->next[0];
    bool first = true;
    while (cur) {
        if (!first) cout << ", ";
        first = false;
        int ival;
        memcpy(&ival, cur->item.value, sizeof(int));
        cout << "(" << cur->item.key << "," << ival << ")";
        cur = cur->next[0];
    }
    if (first) cout << "<empty>";
    cout << "\n";
}

// 삽입 전용 워커: 30번 랜덤 key/value 삽입 (1~1000)
void inserter(Queue* q) {
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(1, 1000);
    for (int i = 0; i < 30; ++i) {
        int key = dist(gen), val = dist(gen);
        Item it{ key, malloc(sizeof(int)), sizeof(int) };
        memcpy(it.value, &val, sizeof(int));
        Reply r = enqueue(q, it);
        {
            lock_guard<mutex> cout_lock(print_mtx);
            cout << "enqueue 수행 : key=" << key
                << "; value=" << val
                << "; success=" << boolalpha << r.success << ";\n";
        }
        printQueue(q);
        free(it.value);
        this_thread::sleep_for(chrono::milliseconds(20));
    }
}

// 삭제 전용 워커: 시작 전에 잠시 대기 후 30번 dequeue 시도
void remover(Queue* q) {
    this_thread::sleep_for(chrono::milliseconds(200));
    for (int i = 0; i < 30; ++i) {
        Reply r = dequeue(q);
        {
            lock_guard<mutex> cout_lock(print_mtx);
            if (r.success) {
                int val;
                memcpy(&val, r.item.value, sizeof(int));
                cout << "dequeue 수행 : key=" << r.item.key
                    << "; value=" << val
                    << "; success=" << boolalpha << r.success << ";\n";
                free(r.item.value);
            }
            else {
                cout << "dequeue 수행 : success=" << boolalpha << r.success << ";\n";
            }
        }
        printQueue(q);
        this_thread::sleep_for(chrono::milliseconds(30));
    }
}

int main() {
    Queue* q = init();

    // 람다 캡처를 사용해 overload error 회피
    thread t1([q]() { inserter(q); });
    thread t2([q]() { remover(q); });

    t1.join();
    t2.join();

    {
        lock_guard<mutex> cout_lock(print_mtx);
        cout << "\n== Final queue state ==\n";
    }
    printQueue(q);

    release(q);
    return 0;
}
