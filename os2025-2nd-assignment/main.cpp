// test_queue_delay.cpp

#include <iostream>
#include <thread>
#include <chrono>
#include "queue.h"

// ť ���θ� ��ȸ�ϸ� (key:value) ���·� ���� ���¸� ����ϴ� �Լ�
void print_queue(Queue* q) {
    Node* curr = q->head;  // head�� ���� ��带 ����Ű�Ƿ�, ù ��°�� dummy
    int idx = 0;

    std::cout << "----- ���� ť ���� ��� ���� -----\n";
    while (curr != nullptr) {
        if (idx == 0) {
            std::cout << "[��� " << idx << "] (dummy)\n";
        }
        else {
            std::cout << "[��� " << idx << "] "
                << "(key:" << curr->item.key
                << ", value:" << (long)(curr->item.value) << ")\n";
        }
        curr = curr->next;
        ++idx;
    }
    std::cout << "------ ť ���� ��� �� ------\n\n";
}

int main() {
    // 1. ť �ʱ�ȭ
    Queue* q = init();
    if (!q) {
        std::cerr << "ť �ʱ�ȭ ����\n";
        return -1;
    }

    // 2. enqueue �� Item �迭 (Ű:��)
    Item test_items[] = {
        {50, (void*)(long)100},
        {20, (void*)(long)200},
        {30, (void*)(long)300},
        {20, (void*)(long)999}, // Ű 20 �ߺ� �� ���� ��� ��ü
        {40, (void*)(long)400}
    };
    constexpr int N = sizeof(test_items) / sizeof(Item);

    // 3. �������� �ϳ��� enqueue �ϸ鼭, 1�ʸ��� ť ���¸� ���
    for (int i = 0; i < N; ++i) {
        Reply r = enqueue(q, test_items[i]);
        std::cout
            << "[Enqueue] "
            << "key=" << test_items[i].key
            << ", value=" << (long)(test_items[i].value)
            << "  �� ���� ����: " << (r.success ? "true" : "false")
            << "\n";

        // enqueue ���� ť ��ü�� ���
        print_queue(q);

        // 1�� ����
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 4. ���������� 1�� ���� �� �� �� �� ť ���� ���
    std::cout << "��� enqueue �Ϸ�, 1�� �� ���� ���� ���\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    print_queue(q);

    // 5. �޸� ����
    release(q);
    return 0;
}
