#ifndef _QTYPE_H  // header guard
#define _QTYPE_H
#include <mutex>
#define MAX_CAPACITY 1024

// ==========이 파일은 수정 가능==========
typedef unsigned int Key;  // 값이 클수록 높은  우선순위
typedef void* Value;


typedef struct {
    Key key;
    Value value;
    size_t value_size;
} Item;


typedef struct {
    bool success;   // true: 성공, false: 실패
    Item item;
    // 필드 추가 가능
} Reply;


// ==========힙 기반 우선순위 큐용 Queue 정의==========
// queue.h에서 요구하는 Node 타입은 사용하지 않지만, 선언은 그대로 두어야 함
typedef struct node_t {
    Item item;
    struct node_t* next;
    // 필드 추가 가능
} Node;


typedef struct {
    Item data[MAX_CAPACITY];
    int size;
    std::mutex  mtx;
} Queue;

// 이후 자유롭게 추가/수정: 새로운 자료형 정의 등

#endif
