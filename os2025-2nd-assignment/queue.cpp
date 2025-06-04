#include <iostream>
#include "queue.h"
#include <atomic>

#define MAX_LEVEL 16

using namespace std;

Queue* init(void) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) return NULL;

    queue->head = (Node*)malloc(sizeof(Node));
    if (queue->head == NULL) {
        free(queue);
        return NULL;
    }

    queue->head->item = { 0, NULL };
    queue->head->level = MAX_LEVEL;
    queue->head->next = (Node**)malloc(sizeof(Node*) * MAX_LEVEL);

    if (queue->head->next == NULL) {
        free(queue->head);
        free(queue);
        return NULL;
    }

    for (int i = 0; i < MAX_LEVEL; i++) {
        queue->head->next[i] = NULL;
    }
    return queue;
}

void release(Queue* queue) {
    if (queue == NULL) return;

    Node* current = queue->head;
    while (current != NULL) {
        Node* next = current->next[0];
        free(current->next);
        free(current);
        current = next;
    }
    free(queue);
}

Node* nalloc(Item item) {
    int level = 1;
    while (rand() % 2 == 0 && level < MAX_LEVEL) level++;

    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) return NULL;

    node->item = item;
    node->level = level;
    node->next = (Node**)malloc(sizeof(Node*) * level);
    if (node->next == NULL) {
        free(node);
        return NULL;
    }

    for (int i = 0; i < level; i++) {
        node->next[i] = NULL;
    }
    return node;
}

void nfree(Node* node) {
    if (node != NULL) {
        free(node->next);
        free(node);
    }
}

Node* nclone(Node* node) {
    if (node == NULL) return NULL;

    Node* cloned = nalloc(node->item);
    if (cloned == NULL) return NULL;

    return cloned;
}

Reply enqueue(Queue* queue, Item item) {
    Reply reply = { false, {0, NULL} };
    if (queue == NULL) return reply;

    Node* new_node = nalloc(item);
    if (new_node == NULL) return reply;

    Node* update[MAX_LEVEL];
    Node* current = queue->head;

    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->next[i] != NULL && current->next[i]->item.key > item.key) {
            current = current->next[i];
        }
        update[i] = current;
    }

    for (int i = 0; i < new_node->level; i++) {
        new_node->next[i] = update[i]->next[i];
        atomic<Node*>* atomic_next = (atomic<Node*>*)(&update[i]->next[i]);
        Node* expected = update[i]->next[i];
        while (!atomic_compare_exchange_strong(atomic_next, &expected, new_node)) {
            expected = update[i]->next[i];
            if (expected != NULL && expected->item.key > item.key) {
                for (int j = 0; j < i; j++) {
                    update[j]->next[j] = new_node->next[j];
                }
                nfree(new_node);
                return enqueue(queue, item);
            }
            new_node->next[i] = expected;
        }
    }

    reply.success = true;
    reply.item = item;

    return reply;
}



Reply dequeue(Queue* queue) {
    Reply reply = { false, {0, NULL} };
    if (queue == NULL || queue->head->next[0] == NULL) return reply;

    Node* update[MAX_LEVEL];
    Node* current = queue->head;

    for (int i = 0; i < MAX_LEVEL; i++) {
        update[i] = queue->head;
    }

    Node* target = queue->head->next[0];
    if (target != NULL) {
        for (int i = 0; i < target->level; i++) {
            atomic<Node*>* atomic_next = (atomic<Node*>*)(&update[i]->next[i]);
            Node* expected = target;
            Node* new_value = target->next[i];
            while (!atomic_compare_exchange_strong(atomic_next, &expected, new_value)) {
                expected = update[i]->next[i];
                if (expected != target) {
                    return dequeue(queue);
                }
            }
        }
        reply.success = true;
        reply.item = target->item;
        nfree(target);
    }

    return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
    if (queue == NULL) return NULL;

    Queue* result = init();
    if (result == NULL) return NULL;

    Node* update[MAX_LEVEL];
    Node* current = queue->head;

    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (current->next[i] != NULL && current->next[i]->item.key > end) {
            current = current->next[i];
        }
        update[i] = current;
    }

    current = current->next[0];
    Node* last[MAX_LEVEL];
    for (int i = 0; i < MAX_LEVEL; i++) {
        last[i] = result->head;
    }

    while (current != NULL && end >= current->item.key && current->item.key >= start) {
        Node* cloned = nclone(current);
        if (cloned == NULL) {
            release(result);
            return NULL;
        }

        for (int i = 0; i < cloned->level; i++) {
            last[i]->next[i] = cloned;
            last[i] = cloned;
        }
        current = current->next[0];
    }

    return result;
}