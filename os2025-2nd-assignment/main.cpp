#include <iostream>
#include <string>
#include <cstring>
#include "queue.h"    // init, enqueue, release 선언 :contentReference[oaicite:0]{index=0}
#include "qtype.h"    // Queue, Node, Item 정의 :contentReference[oaicite:1]{index=1}

using namespace std;

// 바닥 레벨(0)을 순회하며 (key,value) 형식으로 출력.
// value_size가 sizeof(int)이면 정수로, 아니면 문자열로 처리.
void printQueue(Queue* q) {
    cout << "current queue : ";
    Node* cur = q->head->next[0];
    bool first = true;
    while (cur) {
        if (!first) cout << ", ";
        first = false;

        // 정수형 value
        if (cur->item.value && cur->item.value_size == sizeof(int)) {
            int ival;
            memcpy(&ival, cur->item.value, sizeof(int));
            cout << "(" << cur->item.key << "," << ival << ")";
        }
        // 문자열형 value
        else if (cur->item.value) {
            cout << "("
                << cur->item.key
                << ":\""
                << static_cast<char*>(cur->item.value)
                << "\")";
        }
        // 값이 없을 경우
        else {
            cout << "(" << cur->item.key << ",)";
        }

        cur = cur->next[0];
    }
    if (first)               // 한 번도 출력된 적 없으면
        cout << "<empty>";
    cout << "\n";
}

int main() {
    Queue* q = init();

    // 예시로 4가지 아이템(enqueue)을 추가
    struct {
        int key;
        bool isInt;
        const char* str;
        int intval;
    } tests[] = {
        { 100, false, "hello", 0 },
        {  70, true,  nullptr, 30 },
        {  20, true,  nullptr,  5 },
        {  70, true,  nullptr, 5000 }  // 같은 key=70에 대해 값을 5000으로 교체
    };

    for (auto& t : tests) {
        Item it;
        it.key = t.key;
        if (t.isInt) {
            // 정수형 value 메모리 할당
            it.value_size = sizeof(int);
            it.value = malloc(it.value_size);
            memcpy(it.value, &t.intval, sizeof(int));
        }
        else {
            // 문자열형 value 포인터(널 터미널 포함)
            it.value_size = static_cast<int>(strlen(t.str)) + 1;
            it.value = malloc(it.value_size);
            memcpy(it.value, t.str, it.value_size);
        }

        Reply r = enqueue(q, it);
        // 출력 형식 맞추기
        cout << "enqueue 수행 : "
            << "key=" << it.key << "; "
            << "value=";
        if (t.isInt) cout << t.intval;
        else         cout << "\"" << static_cast<char*>(it.value) << "\"";
        cout << "; success=" << boolalpha << r.success << ";\n";

        printQueue(q);

        // 임시로 할당한 value 메모리 해제
        free(it.value);
    }

    release(q);
    return 0;
}
