#include <iostream>
#include <cstdlib>
#include "queue.h"
#include <random>
#define MAX_LEVEL 16

extern float P;
float P = 0.5f;

using namespace std;

int randomLevel() {
    static default_random_engine  engine((unsigned)time(nullptr));
    static uniform_real_distribution<double> dist(0.0, 1.0);

    int lvl = 1;
    while (lvl < MAX_LEVEL && dist(engine) < 0.5) {
        lvl++;
    }
    return lvl;
}


Queue* init(void) {
    Queue* q = new Queue;
    if (!q) {
        return nullptr;
    }

    Node* head = new Node;
    if (!head) {
        delete q;
        return nullptr;
    }
    head->level = MAX_LEVEL;

    head->item.key = 0;
    head->item.value = nullptr;
    head->item.value_size = 0;

    head->next = new Node * [MAX_LEVEL];
    if (!head->next) {
        delete head;
        delete q;
        return nullptr;
    }
    for (int i = 0; i < MAX_LEVEL; i++) {
        head->next[i] = nullptr;
    }

    q->head = head;
    return q;
}

void release(Queue* queue) {
    if (!queue) return;

    Node* head = queue->head;
    if (head) {
        Node* curr = head->next[0];
        while (curr) {
            Node* nextNode = curr->next[0];
            delete[] curr->next;
            delete curr;
            curr = nextNode;
        }
        delete[] head->next;
        delete head;
    }

    delete queue;
}

Node* nalloc(const Item& item) {
    if (item.value == nullptr || item.value_size == 0)
        return nullptr;

    Node* node = new (std::nothrow) Node;
    if (!node)
        return nullptr;

    node->item.key = item.key;
    node->item.value_size = item.value_size;

    if (item.value_size > 0 && item.value != nullptr) {
        char* buffer = new (std::nothrow) char[item.value_size];
        if (!buffer) {
            delete node;
            return nullptr;
        }
        std::memcpy(buffer, item.value, item.value_size);
        node->item.value = static_cast<void*>(buffer);
    }
    else {
        node->item.value = nullptr;
    }

    node->level = randomLevel();
    for (int i = 0; i < MAX_LEVEL; i++) {
        node->next[i] = nullptr;
    }

    return node;
}



void nfree(Node* node) {
    if (!node) return;
    if (node->item.value) {
        delete[] static_cast<char*>(node->item.value);
        node->item.value = nullptr;
    }
    delete node;
}

Node* nclone(Node* node) {
    if (!node) return nullptr;

    Node* clone = new Node;
    if (!clone) {
        return nullptr;
    }

    clone->level = node->level;

    clone->item.key = node->item.key;
    clone->item.value_size = node->item.value_size;

    if (node->item.value_size > 0 && node->item.value != nullptr) {
        char* buffer = new char[node->item.value_size];
        std::memcpy(buffer, node->item.value, node->item.value_size);
        clone->item.value = static_cast<void*>(buffer);
    }
    else {
        clone->item.value = nullptr;
    }

    clone->next = new Node * [clone->level];
    for (int i = 0; i < clone->level; i++) {
        clone->next[i] = node->next[i];
    }

    return clone;
}

Reply enqueue(Queue* queue, Item item) {
    Reply reply = { false, {0, 0, nullptr} };
    if (!queue) return reply;


    if (item.value == nullptr || item.value_size == 0)
        return reply;


    Node* update[MAX_LEVEL];
    Node* curr = queue->head;

    for (int i = queue->level - 1; i >= 0; i--) {
        while (curr->next[i]
            && curr->next[i]->item.key > item.key)
        {
            curr = curr->next[i];
        }
        update[i] = curr;
    }

    Node* possible = curr->next[0];
    if (possible && possible->item.key == item.key) {

        if (possible->item.value) {
            delete[] static_cast<char*>(possible->item.value);
        }

        possible->item.value_size = item.value_size;
        if (item.value_size > 0 && item.value != nullptr) {
            char* buffer = new char[item.value_size];

            std::memcpy(buffer, item.value, item.value_size);
            possible->item.value = static_cast<void*>(buffer);
        }
        else {
            possible->item.value = nullptr;
        }
        reply.success = true;
        reply.item = item;
        return reply;
    }

    Node* new_node = nalloc(item);
    if (!new_node) {
        return reply;
    }
    int new_level = new_node->level;
    if (new_level > queue->level) {
        for (int i = queue->level; i < new_level; i++) {
            update[i] = queue->head;
        }
        queue->level = new_level;
    }

    for (int i = 0; i < new_level; i++) {
        new_node->next[i] = update[i]->next[i];
        update[i]->next[i] = new_node;
    }

    queue->cv.notify_one();

    reply.success = true;
    reply.item = item;
    return reply;
}

Reply dequeue(Queue* queue) {
    Reply reply = { false, {0, nullptr, 0} };
    if (!queue || !(queue->head)) {
        return reply;
    }

    Node* head = queue->head;
    Node* target = head->next[0];

    if (!target) {
        return reply;
    }

    reply.item.key = target->item.key;
    reply.item.value = target->item.value;
    reply.item.value_size = target->item.value_size;
    reply.success = true;

    int lvl = target->level;
    for (int i = 0; i < lvl; i++) {
        if (head->next[i] == target) {
            head->next[i] = target->next[i];
        }
    }

    delete[] target->next;
    delete target;

    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    if (!queue) return nullptr;

    Key low = start < end ? start : end;
    Key high = start < end ? end : start;

    Queue* result = init();
    if (!result) {
        return nullptr;
    }

    Node* curr = queue->head->next[0];

    while (curr && curr->item.key > high) {
        curr = curr->next[0];
    }

    for (; curr && curr->item.key >= low; curr = curr->next[0]) {
        enqueue(result, curr->item);
    }

    return result;
}