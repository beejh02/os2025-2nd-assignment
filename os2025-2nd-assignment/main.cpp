#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <string>
#include "queue.h"

using namespace std;

mutex cout_mutex;

unsigned int generate_random_key() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<unsigned int> dis(1, 100);
    return dis(gen);
}

void thread_task(Queue* queue, int thread_id) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 2); // 0: enqueue, 1: dequeue, 2: range
    string thread_name = "Thread-" + to_string(thread_id);

    int operation = dis(gen);

    switch (operation) {
    case 0: { // enqueue
        unsigned int key = generate_random_key();
        Item item = { key, (int*)key }; // Value = Key
        Reply reply = enqueue(queue, item);
        {
            lock_guard<mutex> lock(cout_mutex);
            if (reply.success) {
                cout << thread_name << ": Enqueued Key = " << reply.item.key
                    << ", Value = " << reply.item.value << endl;
            }
            else {
                cout << thread_name << ": Enqueue failed" << endl;
            }
        }
        break;
    }
    case 1: { // dequeue
        Reply reply = dequeue(queue);
        {
            lock_guard<mutex> lock(cout_mutex);
            if (reply.success) {
                cout << thread_name << ": Dequeued Key = " << reply.item.key
                    << ", Value = " << reply.item.value << endl;
            }
            else {
                cout << thread_name << ": Dequeue failed (queue empty)" << endl;
            }
        }
        break;
    }
    case 2: { // range
        unsigned int start = generate_random_key();
        unsigned int end = start + generate_random_key() % 20;
        Queue* ranged_queue = range(queue, start, end);
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << thread_name << ": Range (" << start << ", " << end << ") created" << endl;
        }

        Reply range_reply = dequeue(ranged_queue);
        {
            lock_guard<mutex> lock(cout_mutex);
            if (range_reply.success) {
                cout << thread_name << ": Range Dequeued Key = " << range_reply.item.key
                    << ", Value = " << range_reply.item.value << endl;
            }
            else {
                cout << thread_name << ": Range queue empty" << endl;
            }
        }
        release(ranged_queue);
        break;
    }
    }
}

int main() {
    Queue* queue = init();
    if (!queue) {
        lock_guard<mutex> lock(cout_mutex);
        cout << "큐 초기화 실패" << endl;
        return 1;
    }

    // 초기 데이터 삽입
    Item item1 = { 10, (int*)10 };
    Item item2 = { 5, (int*)5 };
    Item item3 = { 15, (int*)15 };
    enqueue(queue, item1);
    enqueue(queue, item2);
    enqueue(queue, item3);
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "Main: Enqueued Key = 15, Value = 15" << endl;
        cout << "Main: Enqueued Key = 10, Value = 10" << endl;
        cout << "Main: Enqueued Key = 5, Value = 5" << endl;
    }

    // 10개 스레드 생성
    thread t1(thread_task, queue, 1);
    thread t2(thread_task, queue, 2);
    thread t3(thread_task, queue, 3);
    thread t4(thread_task, queue, 4);
    thread t5(thread_task, queue, 5);
    thread t6(thread_task, queue, 6);
    thread t7(thread_task, queue, 7);
    thread t8(thread_task, queue, 8);
    thread t9(thread_task, queue, 9);
    thread t10(thread_task, queue, 10);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();

    // 최종 큐 상태 출력
    {
        lock_guard<mutex> lock(cout_mutex);
        cout << "\nFinal queue state:" << endl;
        Node* current = queue->head;
        while (current != NULL) {
            cout << "Key = " << current->item.key << ", Value = " << current->item.value << endl;
            current = current->next;
        }
        if (queue->head == NULL) {
            cout << "Queue is empty" << endl;
        }
    }

    release(queue);
    return 0;
}