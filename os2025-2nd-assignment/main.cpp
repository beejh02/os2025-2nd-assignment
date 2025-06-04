#include <iostream>
#include "queue.h"

// 테스트 결과 출력 헬퍼 함수
void print_reply(const Reply& reply, const std::string& operation) {
    if (reply.success) {
        std::cout << operation << " 성공: key=" << reply.item.key
            << ", value=" << *(int*)reply.item.value << std::endl;
    }
    else {
        std::cout << operation << " 실패" << std::endl;
    }
}

// 큐 상태 출력 헬퍼 함수
void print_queue(Queue* queue) {
    if (queue == NULL || queue->head->next[0] == NULL) {
        std::cout << "큐가 비어있습니다." << std::endl;
        return;
    }
    Node* current = queue->head->next[0];
    std::cout << "큐 상태: ";
    while (current != NULL) {
        std::cout << "(key=" << current->item.key << ", value=" << *(int*)current->item.value << ") ";
        current = current->next[0];
    }
    std::cout << std::endl;
}

int main() {
    // 큐 초기화
    Queue* queue = init();
    if (queue == NULL) {
        std::cout << "큐 초기화 실패" << std::endl;
        return 1;
    }
    std::cout << "큐 초기화 성공" << std::endl;

    // 테스트 1: enqueue 테스트
    std::cout << "\n=== enqueue 테스트 ===" << std::endl;
    int values[] = { 'a', 'e', 30, 40, 50}; // 테스트용 값
    for (int i = 0; i < 5; i++) {
        Item item;
        item.key = (i + 1) * 10; // key: 10, 20, 30, 40, 50
        item.value = &values[i];
        Reply reply = enqueue(queue, item);
        print_reply(reply, "enqueue");
    }
    print_queue(queue); // 큐 상태 확인

    // 테스트 2: dequeue 테스트
    std::cout << "\n=== dequeue 테스트 ===" << std::endl;
    for (int i = 0; i < 3; i++) {
        Reply reply = dequeue(queue);
        print_reply(reply, "dequeue");
    }
    print_queue(queue); // dequeue 후 큐 상태 확인

    // 테스트 3: range 테스트
    std::cout << "\n=== range 테스트 ===" << std::endl;
    Queue* range_queue = range(queue, 10, 20); // key가 20~40인 노드 검색
    if (range_queue != NULL) {
        std::cout << "range(10, 20) 성공" << std::endl;
        print_queue(range_queue);
        release(range_queue); // 메모리 해제
    }
    else {
        std::cout << "range(20, 40) 실패" << std::endl;
    }

    // 테스트 4: 빈 큐에서 dequeue 테스트
    std::cout << "\n=== 빈 큐 dequeue 테스트 ===" << std::endl;
    while (queue->head->next[0] != NULL) {
        dequeue(queue); // 큐 비우기
    }
    Reply reply = dequeue(queue); // 빈 큐에서 dequeue
    print_reply(reply, "빈 큐 dequeue");

    // 테스트 5: NULL 큐 테스트
    std::cout << "\n=== NULL 큐 테스트 ===" << std::endl;
    reply = enqueue(NULL, { 10, &values[0] });
    print_reply(reply, "NULL 큐 enqueue");
    reply = dequeue(NULL);
    print_reply(reply, "NULL 큐 dequeue");
    Queue* null_range = range(NULL, 10, 20);
    if (null_range == NULL) {
        std::cout << "NULL 큐 range 실패" << std::endl;
    }

    // 큐 해제
    release(queue);
    std::cout << "\n큐 해제 완료" << std::endl;

    return 0;
}