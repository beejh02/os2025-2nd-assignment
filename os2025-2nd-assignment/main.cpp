// test_queue_range.cpp

#include <iostream>
#include "queue.h"

// ť ���θ� ��ȸ�ϸ� (key:value) ���·� ���� ���¸� ����ϴ� �Լ�
void print_queue(Queue* q) {
    Node* curr = q->head;  // head�� ���� ���(dummy)�� ����Ŵ
    int idx = 0;

    std::cout << "----- ť ���� ��� ���� -----\n";
    while (curr != nullptr) {
        if (idx == 0) {
            // idx == 0�� ���� �׻� dummy ���
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
    // 1) ���� ť �ʱ�ȭ
    Queue* orig_q = init();
    if (!orig_q) {
        std::cerr << "ť �ʱ�ȭ ����\n";
        return -1;
    }

    // 2) ���� ť�� enqueue�� Item �迭 (Ű:��)
    Item test_items[] = {
        {10, (void*)(long)100},
        {30, (void*)(long)300},
        {20, (void*)(long)200},
        {40, (void*)(long)400},
        {25, (void*)(long)250}
    };
    constexpr int N = sizeof(test_items) / sizeof(Item);

    // 3) ���� ť�� ������� enqueue
    std::cout << "*** ���� ť�� �������� enqueue �մϴ� ***\n";
    for (int i = 0; i < N; ++i) {
        Reply r = enqueue(orig_q, test_items[i]);
        std::cout
            << "[Enqueue] "
            << "key=" << test_items[i].key
            << ", value=" << (long)(test_items[i].value)
            << "  �� ���� ����: " << (r.success ? "true" : "false")
            << "\n";
    }
    std::cout << "\n";

    // 4) ���� ť ��ü ���� ���
    std::cout << "*** ���� ť ��ü ���� ***\n";
    print_queue(orig_q);

    // 5) range �Լ� �׽�Ʈ: start=15, end=35 ���� ����
    Key start = 15;
    Key end = 35;
    std::cout
        << "*** range(orig_q, start=" << start
        << ", end=" << end << ") ȣ�� ***\n";

    Queue* ranged_q = range(orig_q, start, end);

    // 6) range ��� ť ���� ���
    std::cout << "\n*** range�� �� ť ����(Ű�� "
        << start << " �̻�, " << end << " ������ ��常) ***\n";
    print_queue(ranged_q);

    // 7) ���� ť(Test ���Ŀ��� ���� ���� ����) ���� ��Ȯ��
    std::cout << "*** ���� ť�� ������� �ʾҴ��� ��Ȯ�� ***\n";
    print_queue(orig_q);

    // 8) �޸� ����
    release(ranged_q);   // range�� ������ �� ť ����
    release(orig_q);     // ���� ť ����

    return 0;
}
