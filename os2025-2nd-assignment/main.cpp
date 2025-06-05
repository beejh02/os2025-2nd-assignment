/**
 * main.cpp
 *
 * ���� ������, enqueue(queue, 10) ȣ�⸸ �����ϵ��� ���� �����Դϴ�.
 * Value�� ������� �ʰ�, key�� �ִ� �����Դϴ�.
 *
 * - enqueue(Queue*, Key) �����ε带 �߰��Ͽ�, Item�� ���� ���� �ʿ� ����
 *   enqueue(queue, 10) ���� ȣ���� �����ϵ��� �߽��ϴ�.
 * - ���������� value�� nullptr, value_size�� 0���� �����˴ϴ�.
 */

#include <iostream>
#include "queue.h"

using namespace std;

Reply enqueue(Queue* q, Key key) {
    Item it;
    it.key = key;
    it.value = nullptr;
    it.value_size = 0;
    return enqueue(q, it);
}

int main() {
    Queue* q = init();
    if (!q) {
        cerr << "ť �ʱ�ȭ ����\n";
        return 1;
    }

    Reply r = enqueue(q, 10);
    if (r.success) {
        cout << "enqueue ����: key = 10\n";
    }
    else {
        cout << "enqueue ����\n";
    }

    Reply d = dequeue(q);
    if (d.success) {
        cout << "dequeue ����: key = " << d.item.key << "\n";
    }
    else {
        cout << "dequeue ����(ť�� ��� �ְų� ����)\n";
    }

    release(q);
    return 0;
}
