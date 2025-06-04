#include <iostream>
#include "queue.h"

// �׽�Ʈ ��� ��� ���� �Լ�
void print_reply(const Reply& reply, const std::string& operation) {
    if (reply.success) {
        std::cout << operation << " ����: key=" << reply.item.key
            << ", value=" << *(int*)reply.item.value << std::endl;
    }
    else {
        std::cout << operation << " ����" << std::endl;
    }
}

// ť ���� ��� ���� �Լ�
void print_queue(Queue* queue) {
    if (queue == NULL || queue->head->next[0] == NULL) {
        std::cout << "ť�� ����ֽ��ϴ�." << std::endl;
        return;
    }
    Node* current = queue->head->next[0];
    std::cout << "ť ����: ";
    while (current != NULL) {
        std::cout << "(key=" << current->item.key << ", value=" << *(int*)current->item.value << ") ";
        current = current->next[0];
    }
    std::cout << std::endl;
}

int main() {
    // ť �ʱ�ȭ
    Queue* queue = init();
    if (queue == NULL) {
        std::cout << "ť �ʱ�ȭ ����" << std::endl;
        return 1;
    }
    std::cout << "ť �ʱ�ȭ ����" << std::endl;

    // �׽�Ʈ 1: enqueue �׽�Ʈ
    std::cout << "\n=== enqueue �׽�Ʈ ===" << std::endl;
    int values[] = { 'a', 'e', 30, 40, 50}; // �׽�Ʈ�� ��
    for (int i = 0; i < 5; i++) {
        Item item;
        item.key = (i + 1) * 10; // key: 10, 20, 30, 40, 50
        item.value = &values[i];
        Reply reply = enqueue(queue, item);
        print_reply(reply, "enqueue");
    }
    print_queue(queue); // ť ���� Ȯ��

    // �׽�Ʈ 2: dequeue �׽�Ʈ
    std::cout << "\n=== dequeue �׽�Ʈ ===" << std::endl;
    for (int i = 0; i < 3; i++) {
        Reply reply = dequeue(queue);
        print_reply(reply, "dequeue");
    }
    print_queue(queue); // dequeue �� ť ���� Ȯ��

    // �׽�Ʈ 3: range �׽�Ʈ
    std::cout << "\n=== range �׽�Ʈ ===" << std::endl;
    Queue* range_queue = range(queue, 10, 20); // key�� 20~40�� ��� �˻�
    if (range_queue != NULL) {
        std::cout << "range(10, 20) ����" << std::endl;
        print_queue(range_queue);
        release(range_queue); // �޸� ����
    }
    else {
        std::cout << "range(20, 40) ����" << std::endl;
    }

    // �׽�Ʈ 4: �� ť���� dequeue �׽�Ʈ
    std::cout << "\n=== �� ť dequeue �׽�Ʈ ===" << std::endl;
    while (queue->head->next[0] != NULL) {
        dequeue(queue); // ť ����
    }
    Reply reply = dequeue(queue); // �� ť���� dequeue
    print_reply(reply, "�� ť dequeue");

    // �׽�Ʈ 5: NULL ť �׽�Ʈ
    std::cout << "\n=== NULL ť �׽�Ʈ ===" << std::endl;
    reply = enqueue(NULL, { 10, &values[0] });
    print_reply(reply, "NULL ť enqueue");
    reply = dequeue(NULL);
    print_reply(reply, "NULL ť dequeue");
    Queue* null_range = range(NULL, 10, 20);
    if (null_range == NULL) {
        std::cout << "NULL ť range ����" << std::endl;
    }

    // ť ����
    release(queue);
    std::cout << "\nť ���� �Ϸ�" << std::endl;

    return 0;
}