#ifndef _QTYPE_H  // header guard
#define _QTYPE_H
#include <mutex>
#define MAX_CAPACITY 1024

// ==========�� ������ ���� ����==========
typedef unsigned int Key;  // ���� Ŭ���� ����  �켱����
typedef void* Value;


typedef struct {
    Key key;
    Value value;
    size_t value_size;
} Item;


typedef struct {
    bool success;   // true: ����, false: ����
    Item item;
    // �ʵ� �߰� ����
} Reply;


// ==========�� ��� �켱���� ť�� Queue ����==========
// queue.h���� �䱸�ϴ� Node Ÿ���� ������� ������, ������ �״�� �ξ�� ��
typedef struct node_t {
    Item item;
    struct node_t* next;
    // �ʵ� �߰� ����
} Node;


typedef struct {
    Item data[MAX_CAPACITY];
    int size;
    std::mutex  mtx;
} Queue;

// ���� �����Ӱ� �߰�/����: ���ο� �ڷ��� ���� ��

#endif
