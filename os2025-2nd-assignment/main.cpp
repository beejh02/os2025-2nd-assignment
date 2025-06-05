/**
 * main.cpp
 *
 * 아주 간단히, enqueue(queue, 10) 호출만 가능하도록 만든 예제입니다.
 * Value는 사용하지 않고, key만 넣는 형태입니다.
 *
 * - enqueue(Queue*, Key) 오버로드를 추가하여, Item을 직접 만들 필요 없이
 *   enqueue(queue, 10) 같은 호출이 가능하도록 했습니다.
 * - 내부적으로 value는 nullptr, value_size는 0으로 설정됩니다.
 */

#include <iostream>
#include "queue.h"

using namespace std;

Reply enqueue(Queue* q, Key key) {
    Item it;
    it.key = key;
    it.value = nullptr;
    it.value_size = 0;
    return enqueue(q, it);
}

int main() {
    Queue* q = init();
    if (!q) {
        cerr << "큐 초기화 실패\n";
        return 1;
    }

    Reply r = enqueue(q, 10);
    if (r.success) {
        cout << "enqueue 성공: key = 10\n";
    }
    else {
        cout << "enqueue 실패\n";
    }

    Reply d = dequeue(q);
    if (d.success) {
        cout << "dequeue 성공: key = " << d.item.key << "\n";
    }
    else {
        cout << "dequeue 실패(큐가 비어 있거나 오류)\n";
    }

    release(q);
    return 0;
}
