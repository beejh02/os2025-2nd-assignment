#ifndef _QTYPE_H  // header guard
#define _QTYPE_H
#include <atomic>
#include <condition_variable>

// ==========�� ������ ���� ����==========

typedef unsigned int Key;  // ���� Ŭ���� ���� �켱����
typedef void* Value;

typedef struct {
    Key key;
    Value value;
    int value_size;
} Item;

typedef struct {
    bool success;   // true: ����, false: ����
    Item item;
    // �ʵ� �߰� ����
} Reply;

typedef struct node_t {
    Item item;
    int level;
    struct node_t** next;
    // �ʵ� �߰� ����
} Node;

typedef struct {
    Node* head;
    int level;
    std::mutex mtx;
    std::condition_variable cv;
} Queue;

// ���� �����Ӱ� �߰�/����: ���ο� �ڷ��� ���� ��

#endif
