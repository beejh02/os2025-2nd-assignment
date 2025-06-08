#include <iostream>
#include <thread>
#include <mutex>
#include <random>
#include <chrono>
#include <sstream>
#include "queue.h"     // init, enqueue, dequeue, release 선언
#include "qtype.h"     // Queue, Node, Item 정의

using namespace std;

// 모든 큐 연산과 로그를 원자적으로 묶기 위한 mutex
mutex op_mtx;

// 큐 스냅샷 문자열 생성 (바닥 레벨 탐색, q->mtx 보호)
string snapshotQueue(Queue* q) {
    ostringstream oss;
    lock_guard<mutex> lock(q->mtx);
    oss << "current queue : ";
    Node* cur = q->head->next[0];
    bool first = true;
    while (cur) {
        if (!first) oss << ", ";
        first = false;
        int v;
        memcpy(&v, cur->item.value, sizeof(int));
        oss << "(" << cur->item.key << "," << v << ")";
        cur = cur->next[0];
    }
    if (first) oss << "<empty>";
    return oss.str();
}

// 삽입 전용 워커: 30번 랜덤 key/value 삽입 (1~1000)
void inserter(Queue* q) {
    mt19937 gen(random_device{}());
    uniform_int_distribution<int> dist(1, 1000);
    for (int i = 0; i < 30; ++i) {
        int key = dist(gen);
        int val = dist(gen);
        Item it{ key, malloc(sizeof(int)), sizeof(int) };
        memcpy(it.value, &val, sizeof(int));

        // 연산과 로그를 한 번에 묶어 순서 보장
        {
            lock_guard<mutex> lock(op_mtx);
            Reply r = enqueue(q, it);
            cout << "enqueue 수행 : key=" << key
                << "; value=" << val
                << "; success=" << boolalpha << r.success << "\n";
            cout << snapshotQueue(q) << "\n";
        }

        free(it.value);
        this_thread::sleep_for(chrono::milliseconds(20));
    }
}

// 삭제 전용 워커: 시작 전에 잠시 대기 후 30번 dequeue 시도
void remover(Queue* q) {
    this_thread::sleep_for(chrono::milliseconds(200));
    for (int i = 0; i < 30; ++i) {
        // 연산과 로그를 한 번에 묶어 순서 보장
        {
            lock_guard<mutex> lock(op_mtx);
            Reply r = dequeue(q);
            if (r.success) {
                int val;
                memcpy(&val, r.item.value, sizeof(int));
                cout << "dequeue 수행 : key=" << r.item.key
                    << "; value=" << val
                    << "; success=" << boolalpha << r.success << "\n";
            }
            else {
                cout << "dequeue 수행 : success=" << boolalpha << r.success << "\n";
            }
            cout << snapshotQueue(q) << "\n";
            if (r.success) free(r.item.value);
        }

        this_thread::sleep_for(chrono::milliseconds(30));
    }
}

int main() {
    Queue* q = init();

    thread t1([q]() { inserter(q); });
    thread t2([q]() { remover(q); });

    t1.join();
    t2.join();

    // 최종 상태도 원자적으로 출력
    {
        lock_guard<mutex> lock(op_mtx);
        cout << "\n== Final queue state ==\n";
        cout << snapshotQueue(q) << "\n";
    }

    release(q);
    return 0;
}
