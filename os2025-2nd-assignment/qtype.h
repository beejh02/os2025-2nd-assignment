#ifndef _QTYPE_H  // header guard
#define _QTYPE_H
#include <atomic>

// ==========�� ������ ���� ����==========

using namespace std;

typedef unsigned int Key;  // ���� Ŭ���� ���� �켱����
typedef void* Value;

typedef struct {
    Key key;
    Value value;
} Item;

typedef struct {
    bool success;   // true: ����, false: ����
    Item item;
    // �ʵ� �߰� ����
} Reply;

typedef struct node_t {
    Item item;
    atomic<node_t*> next;
    // �ʵ� �߰� ����
} Node;

typedef struct {
    atomic<Node*> head;
} Queue;

// ���� �����Ӱ� �߰�/����: ���ο� �ڷ��� ���� ��

#endif
