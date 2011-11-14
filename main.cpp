#include <algorithm>
#include <queue>
#include <iostream>
#include <string>
#include <map>
#include <cassert>
#include "heap.h"
typedef LeftHeap<int, std::string, std::greater<int> > heap;

std::string genstr() {
	std::string a;
	for (int i = 0; i < 7; ++i) {
		a += char(rand() % 26 + 'a');
	}
	return a;
}

void validate(heap::Node* node) {
	if (node->l == 0)
		assert(node->r == 0);
	assert(node->r == 0 || node->l == 0 || node->r->height <= node->l->height);
	if (node->l) {
		validate(node->l);
		assert(node->l->value < node->value);
	}
	if (node->r) {
		assert(node->r->value < node->value);
		assert(node->height == node->r->height + 1);
		validate(node->r);
	}
}

void StressTest() {

	heap lh;
	int expectedSize = 0;
	std::priority_queue<int> pq;
	srand(time(0));
	std::map<int, std::string> m;
	for (int i = 0; i < 100000; ++i) {
		if ((rand() % 10) < 6) {
			int nextInt = rand();
			if (m.find(nextInt) != m.end())
				continue;
			std::string nextStr = genstr();
			m[nextInt] = nextStr;
			++expectedSize;
			lh.push(nextInt, nextStr);
			assert(lh.size() == expectedSize);
			pq.push(nextInt);
			assert(lh.top() == m[pq.top()]);
		} else {
			if (expectedSize == 0)
				continue;
			--expectedSize;
			m.erase(pq.top());
			pq.pop();
			lh.pop();
			assert(lh.size() == expectedSize);
			if (expectedSize) {
				assert(lh.top() == m[pq.top()]);
			}
		}
		if (expectedSize)
			validate(lh.root_);
	}
}

int main() {
	StressTest();
	return 0;
}
