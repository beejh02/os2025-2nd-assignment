// test_queue_range.cpp

#include <iostream>
#include "queue.h"

// 큐 내부를 순회하며 (key:value) 형태로 현재 상태를 출력하는 함수
void print_queue(Queue* q) {
    Node* curr = q->head;  // head는 더미 노드(dummy)를 가리킴
    int idx = 0;

    std::cout << "----- 큐 상태 출력 시작 -----\n";
    while (curr != nullptr) {
        if (idx == 0) {
            // idx == 0일 때는 항상 dummy 노드
            std::cout << "[노드 " << idx << "] (dummy)\n";
        }
        else {
            std::cout << "[노드 " << idx << "] "
                << "(key:" << curr->item.key
                << ", value:" << (long)(curr->item.value) << ")\n";
        }
        curr = curr->next;
        ++idx;
    }
    std::cout << "------ 큐 상태 출력 끝 ------\n\n";
}

int main() {
    // 1) 원본 큐 초기화
    Queue* orig_q = init();
    if (!orig_q) {
        std::cerr << "큐 초기화 실패\n";
        return -1;
    }

    // 2) 원본 큐에 enqueue할 Item 배열 (키:값)
    Item test_items[] = {
        {10, (void*)(long)100},
        {30, (void*)(long)300},
        {20, (void*)(long)200},
        {40, (void*)(long)400},
        {25, (void*)(long)250}
    };
    constexpr int N = sizeof(test_items) / sizeof(Item);

    // 3) 원본 큐에 순서대로 enqueue
    std::cout << "*** 원본 큐에 아이템을 enqueue 합니다 ***\n";
    for (int i = 0; i < N; ++i) {
        Reply r = enqueue(orig_q, test_items[i]);
        std::cout
            << "[Enqueue] "
            << "key=" << test_items[i].key
            << ", value=" << (long)(test_items[i].value)
            << "  → 성공 여부: " << (r.success ? "true" : "false")
            << "\n";
    }
    std::cout << "\n";

    // 4) 원본 큐 전체 상태 출력
    std::cout << "*** 원본 큐 전체 상태 ***\n";
    print_queue(orig_q);

    // 5) range 함수 테스트: start=15, end=35 범위 설정
    Key start = 15;
    Key end = 35;
    std::cout
        << "*** range(orig_q, start=" << start
        << ", end=" << end << ") 호출 ***\n";

    Queue* ranged_q = range(orig_q, start, end);

    // 6) range 결과 큐 상태 출력
    std::cout << "\n*** range된 새 큐 상태(키가 "
        << start << " 이상, " << end << " 이하인 노드만) ***\n";
    print_queue(ranged_q);

    // 7) 원본 큐(Test 이후에도 아직 남아 있음) 상태 재확인
    std::cout << "*** 원본 큐가 변경되지 않았는지 재확인 ***\n";
    print_queue(orig_q);

    // 8) 메모리 해제
    release(ranged_q);   // range로 생성된 새 큐 해제
    release(orig_q);     // 원본 큐 해제

    return 0;
}
