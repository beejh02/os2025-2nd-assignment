// test_queue_with_dequeue.cpp

#include <iostream>
#include <thread>
#include <chrono>
#include "queue.h"

// ─────────────────────────────────────────────────────────────────────────────
// 큐 내부를 순회하며 (key:value) 형태로 현재 상태를 출력하는 함수
// ─────────────────────────────────────────────────────────────────────────────
void print_queue(Queue* q) {
    Node* curr = q->head;  // head는 Dummy 노드를 가리킴
    int idx = 0;

    std::cout << "----- 현재 큐 상태 출력 시작 -----\n";
    while (curr != nullptr) {
        if (idx == 0) {
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
    // 1) 큐 초기화
    Queue* q = init();
    if (!q) {
        std::cerr << "큐 초기화 실패\n";
        return -1;
    }

    // 2) enqueue할 Item 배열 (키:값)
    Item test_items[] = {
        {50, (void*)(long)100},
        {20, (void*)(long)200},
        {30, (void*)(long)300},
        {20, (void*)(long)999}, // 키 20 중복 → 기존 노드 교체
        {40, (void*)(long)400}
    };
    constexpr int N = sizeof(test_items) / sizeof(Item);

    // 3) 아이템을 하나씩 enqueue 하면서, 1초마다 큐 상태 출력
    for (int i = 0; i < N; ++i) {
        Reply r = enqueue(q, test_items[i]);
        std::cout
            << "[Enqueue] "
            << "key=" << test_items[i].key
            << ", value=" << (long)(test_items[i].value)
            << "  → 성공 여부: " << (r.success ? "true" : "false")
            << "\n";

        // enqueue 직후 큐 전체 출력
        print_queue(q);

        // 1초 지연
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 4) 마지막 enqueue 이후 1초 대기 및 큐 상태 출력
    std::cout << "모든 enqueue 완료, 1초 후 최종 상태 출력\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    print_queue(q);

    // ─────────────────────────────────────────────────────────────────────────
    // 5) Dequeue 테스트: 큐가 비어 있을 때까지 1초마다 dequeue 수행
    // ─────────────────────────────────────────────────────────────────────────
    std::cout << "----- 이제 dequeue를 1초 간격으로 수행하며 결과 확인 -----\n";
    while (true) {
        // dequeue 호출
        Reply dr = dequeue(q);
        if (!dr.success) {
            // 큐가 비어있으면 반복 종료
            std::cout << "[Dequeue] 실패: 큐가 비어있습니다.\n";
            break;
        }

        // 정상적으로 꺼낸 아이템 출력
        std::cout
            << "[Dequeue] "
            << "key=" << dr.item.key
            << ", value=" << (long)(dr.item.value)
            << "  → 성공 여부: " << (dr.success ? "true" : "false")
            << "\n";

        // dequeue 직후 큐 상태 출력
        print_queue(q);

        // 1초 지연
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 6) 최종 상태 확인 (완전히 비워졌는지)
    std::cout << "모든 dequeue 완료, 큐가 비어 있는지 확인 중...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    print_queue(q);

    // 7) 메모리 해제
    release(q);
    return 0;
}
