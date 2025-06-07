#ifndef _QTYPE_H  // header guard
#define _QTYPE_H
#define MAX_LEVEL 16
#define P_FACTOR 0.5f
#include <mutex>

using namespace std;

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
    node_t** next;
    // �ʵ� �߰� ����
} Node;

typedef struct {
    Node* head;
    mutex mtx;
} Queue;

// ���� �����Ӱ� �߰�/����: ���ο� �ڷ��� ���� ��

#endif
