#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <cstdlib>
#include <cmath>
#include <ctime>

int bcnt = 0;

template <typename T>
class bhnode
{
public:
	T x;
	std::vector <bhnode*> s;
	bhnode (T key);
};

template <typename T>
bhnode<T>::bhnode (T key): x(key), s(0){bcnt++;}

template <typename T>
bhnode<T>* bhnodemerge (bhnode<T> *a, bhnode<T> *b)
{
	if (a == NULL || b == NULL) return std::max(a, b);
	if (a->x > b->x) std::swap(a, b);
	a->s.push_back(b);
	return a;
}

template <typename T>
class bheap
{
private:
	std::vector <bhnode<T>*> heap;
	bheap (T key);
	void update ();
public:
	bheap ();
	void merge (bheap &h);
	void push (T key);
	T top ()const;
	void pop ();
	bool empty ()const;
};

template <typename T>
bheap<T>::bheap (): heap(0){}
template <typename T>
bheap<T>::bheap (T key): heap(1, new bhnode<T>(key)){}

template <typename T>
void bheap<T>::update ()
{
	size_t sz = heap.size();
	while (sz > 0 && heap[sz - 1] == NULL)
		sz--;
	heap.resize(sz);
}

template <typename T>
void bheap<T>::merge (bheap<T> &h)
{
	if (h.heap.size() == 0)
		return;
	if (heap.size() == 0)
	{
		heap = h.heap;
		h.heap.resize(0);
		return;
	}
		
	heap.resize(std::max(heap.size(), h.heap.size()) + 1, NULL);
	h.heap.resize(heap.size(), NULL);
	bhnode<T> *nd = NULL, *bf;
	
	for (size_t i = 0; i < heap.size(); ++i)
	{
		if (heap[i] == NULL)
			if (nd == NULL || h.heap[i] == NULL)
				heap[i] = std::max(nd, h.heap[i]), nd = NULL;
			else
				nd = bhnodemerge(h.heap[i], nd), heap[i] = NULL;
		else
			if (nd != NULL || h.heap[i] != NULL)
				bf = std::min(nd, h.heap[i]), 
				nd = bhnodemerge(std::max(nd, h.heap[i]), heap[i]),
				heap[i] = bf;
	}
	update();
	h.heap.resize(0);
}

template <typename T>
void bheap<T>::push (T key)
{
	bheap<T> h(key);
	merge(h);
}

template <typename T>
T bheap<T>::top ()const
{
	assert(heap.size() > 0 /** "Heap has no elements!" **/ );
	T Min = heap.back()->x;
	for (size_t i = 0; i < heap.size(); ++i)
		if (heap[i] != NULL)
			Min = std::min(Min, heap[i]->x);
	return Min;
}

template <typename T>
void bheap<T>::pop ()
{
	assert(heap.size() > 0 /** "Heap has no elements!" **/ );
	size_t MinInd = heap.size() - 1;
	for (size_t i = 0; i < heap.size(); ++i)
		if (heap[i] != NULL && heap[i]->x < heap[MinInd]->x)
			MinInd = i;
	
	bheap<T> h;
	h.heap.resize(MinInd, NULL);
	for (size_t i = 0; i < MinInd; ++i)
		h.heap[i] = heap[MinInd]->s[i];
	heap[MinInd] = NULL;
	merge(h);
}

template <typename T>
bool bheap<T> :: empty ()const
{
	return heap.size() == 0;
}

std :: priority_queue <int> q;
bheap <int> h;

int main ()
{
	time_t st = std::clock();
	for (int i = 0; i < 1000000; ++i)
	{
		int x = rand() - (RAND_MAX / 2);
		h.push(x);
		q.push(-x);
		assert(h.top() == -q.top());
	}
	
	for (int i = 0; i < 1000000 - 1; ++i)
	{
		h.pop();
		q.pop();
		assert(h.top() == -q.top());
	}
	std::cerr << (clock() - st) / double(CLOCKS_PER_SEC);
	return 0;
}
