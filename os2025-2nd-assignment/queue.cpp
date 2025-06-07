#include <iostream>
#include <thread>
#include <ctime>
#include "queue.h"

// randLevel은 인공지능의 도움을 받아 작성
static int randLevel() {
    thread_local unsigned int seed = [] {
        auto t = static_cast<unsigned long>(time(nullptr));
        auto id_hash = hash<thread::id>{}(this_thread::get_id()); // thread 별 id로 각 스레드는 각 하나의 난수만 가지게
        return static_cast<unsigned int>(t ^ id_hash);
    }();

    int lvl = 1;

    while (lvl < MAX_LEVEL) {
        seed = seed * 321232123 + 12345;
        float r = static_cast<float>((seed >> 16) & 0x7FFF) / 32767.0f; // bit shift로 16 미만의 비트 추출
        if (r < P_FACTOR) {
            ++lvl;
        } else {
            break;
        }
    }
    return lvl;
}

Queue* init(void) {
	srand((unsigned)time(nullptr));
	Queue* q = new Queue;
	Node* head = new Node;
	head->level = MAX_LEVEL;
	head->item.key = 0;
	head->item.value = nullptr;
	head->item.value_size = 0;
	head->next = new Node * [MAX_LEVEL];
	for (int i = 0; i < MAX_LEVEL; ++i) {
		head->next[i] = nullptr;
	}
	q->head = head;
	return q;
}


void release(Queue* queue) {
	if (!queue) return;

	Node* cur = queue->head->next[0];
	while (cur) {
		Node* nxt = cur->next[0];
		nfree(cur);
		cur = nxt;
	}

	delete[] queue->head->next;
	delete queue->head;
	delete queue;
}


Node* nalloc(Item item) {
	int lvl = randLevel();
	Node* node = new Node;


	node->item.key = item.key;
	node->item.value_size = item.value_size;

	if (item.value_size > 0 && item.value) {
		node->item.value = malloc(item.value_size);
		if (!node->item.value) {
			delete node;
			return nullptr;
		}
		memcpy(node->item.value, item.value, item.value_size);
	}
	else {
		node->item.value = nullptr;
	}

	node->level = lvl;
	node->next = static_cast<Node**>(calloc(lvl, sizeof(Node*)));
	if (!node->next) {
		free(node->item.value);
		delete node;
		return nullptr;
	}

	return node;
}


void nfree(Node* node) {
	if (!node) return;

	if (node->item.value) {
		free(node->item.value);
	}

	free(node->next);
	delete node;
}


Node* nclone(Node* orig) {
	if (!orig) return nullptr;
	Node* node = new Node;
	
	node->item.key = orig->item.key;
	node->item.value_size = orig->item.value_size;
	if (orig->item.value_size > 0) {
		char* buf = new char[orig->item.value_size];
		memcpy(buf, orig->item.value, orig->item.value_size);
		node->item.value = buf;
		
	}
	else {
		node->item.value = nullptr;
		
	}
	node->level = orig->level;
	node->next = new Node*[node->level]();

	return node;
}

Reply enqueue(Queue* queue, Item item) {
	Reply reply{ false, item };
	if (!queue) return reply;

	lock_guard<mutex> lock(queue->mtx);

	Node* update[MAX_LEVEL];
	Node* x = queue->head;
	for (int i = MAX_LEVEL - 1; i >= 0; --i) {
		while (x->next[i] && x->next[i]->item.key > item.key) {
			x = x->next[i];
		}
		update[i] = x;
	}

	Node* next0 = update[0]->next[0];
	if (next0 && next0->item.key == item.key) {
		free(next0->item.value);
		next0->item.value = nullptr;
		next0->item.value_size = 0;

		if (item.value && item.value_size > 0) {
			next0->item.value = malloc(item.value_size);
			if (next0->item.value) {
				memcpy(next0->item.value, item.value, item.value_size);
				next0->item.value_size = item.value_size;
				reply.success = true;
			}
		}
		return reply;
	}

	Node* node = nalloc(item);
	if (!node) return reply;

	for (int i = 0; i < node->level; ++i) {
		node->next[i] = update[i]->next[i];
		update[i]->next[i] = node;
	}

	reply.success = true;
	return reply;
}

Reply dequeue(Queue* queue) {
	if (!queue)		return { false, { 0, nullptr, 0 } };

	lock_guard<mutex> lock(queue->mtx);

	Node* target = queue->head->next[0];
	if (!target)	return { false, { 0, nullptr, 0 } };

	for (int i = 0; i < target->level; ++i) {
		if (queue->head->next[i] == target) {
			queue->head->next[i] = target->next[i];
		}
	}

	Reply reply{ true, target->item };
	nfree(target);
	return reply;
}

Queue* range(Queue* queue, Key start, Key end) {
	if (!queue) return nullptr;

	lock_guard<mutex> lock(queue->mtx);

	Queue* newQ = init();
	if (!newQ) return nullptr;

	Node* cur = queue->head->next[0];
	while (cur && cur->item.key > end) {
		cur = cur->next[0];
	}

	for (; cur && cur->item.key >= start; cur = cur->next[0]) {
		Node* clone = nclone(cur);

		Node* update[MAX_LEVEL];
		Node* x = newQ->head;
		for (int i = MAX_LEVEL - 1; i >= 0; --i) {
			while (x->next[i] && x->next[i]->item.key > clone->item.key) {
				x = x->next[i];
			}
			update[i] = x;
		}

		for (int i = 0; i < clone->level; ++i) {
			clone->next[i] = update[i]->next[i];
			update[i]->next[i] = clone;
		}
	}

	return newQ;
}
