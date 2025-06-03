// test_queue_delay.cpp

#include <iostream>
#include <thread>
#include <chrono>
#include "queue.h"

// 큐 내부를 순회하며 (key:value) 형태로 현재 상태를 출력하는 함수
void print_queue(Queue* q) {
    Node* curr = q->head;  // head는 더미 노드를 가리키므로, 첫 번째는 dummy
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
    // 1. 큐 초기화
    Queue* q = init();
    if (!q) {
        std::cerr << "큐 초기화 실패\n";
        return -1;
    }

    // 2. enqueue 할 Item 배열 (키:값)
    Item test_items[] = {
        {50, (void*)(long)100},
        {20, (void*)(long)200},
        {30, (void*)(long)300},
        {20, (void*)(long)999}, // 키 20 중복 → 기존 노드 교체
        {40, (void*)(long)400}
    };
    constexpr int N = sizeof(test_items) / sizeof(Item);

    // 3. 아이템을 하나씩 enqueue 하면서, 1초마다 큐 상태를 출력
    for (int i = 0; i < N; ++i) {
        Reply r = enqueue(q, test_items[i]);
        std::cout
            << "[Enqueue] "
            << "key=" << test_items[i].key
            << ", value=" << (long)(test_items[i].value)
            << "  → 성공 여부: " << (r.success ? "true" : "false")
            << "\n";

        // enqueue 직후 큐 전체를 출력
        print_queue(q);

        // 1초 지연
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 4. 마지막으로 1초 지연 후 한 번 더 큐 상태 출력
    std::cout << "모든 enqueue 완료, 1초 후 최종 상태 출력\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    print_queue(q);

    // 5. 메모리 해제
    release(q);
    return 0;
}
