#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <cstdlib>
#include <cmath>
#include <ctime>

/**
 * К сожалению, убунта на ноуте легла, 
 * и мне стремно разбираться как тоже самое(GitHub) запилить на винде.
 * Поэтому, я не могу создать отдельный хедер как и любой другой файл.
 * Добавил тест merge'а.
**/

template <typename ValueType>
class BinHeapNode
{
public:
	ValueType _Value;
	std::vector <BinHeapNode*> _Son;
	explicit BinHeapNode (ValueType Value);
	explicit BinHeapNode (BinHeapNode *h);
	~BinHeapNode ();
};

template <typename ValueType>
BinHeapNode<ValueType> :: BinHeapNode (ValueType Value): _Value(Value), _Son(0){}

template <typename ValueType>
BinHeapNode<ValueType> :: BinHeapNode (BinHeapNode *h)
{
	_Value = h->_Value;
	_Son.resize(h->_Son.size(), NULL);
	for (size_t i = 0; i < _Son.size(); ++i)
		if (h->_Son[i] != NULL)
			_Son[i] = new BinHeapNode(h->_Son[i]);
}

template <typename ValueType>
BinHeapNode<ValueType> :: ~BinHeapNode ()
{
	for (size_t i = 0; i < _Son.size(); ++i)
		if (_Son[i] != NULL)
			delete _Son[i];
}

template <typename ValueType>
BinHeapNode<ValueType>* BinHeapNodeMerge (BinHeapNode<ValueType> *a, BinHeapNode<ValueType> *b)
{
	if (a == NULL || b == NULL) return a == NULL ? b : a;
	if (a->_Value > b->_Value) std::swap(a, b);
	a->_Son.push_back(b);
	return a;
}

template <typename ValueType>
class BinHeap
{
private:
	std::vector <BinHeapNode<ValueType>*> _Heap;
	void Clean ();
	size_t FindMinPosition ()const;
public:
	BinHeap ();
	explicit BinHeap (ValueType Value);
	explicit BinHeap (const BinHeap<ValueType> &h);
	~BinHeap ();
	void merge (BinHeap &h);
	void push (ValueType Value);
	ValueType top ()const;
	void pop ();
	bool empty ()const;
};

template <typename ValueType>
BinHeap<ValueType> :: BinHeap (): _Heap(0){}
template <typename ValueType>
BinHeap<ValueType> :: BinHeap (ValueType Value): _Heap(1, new BinHeapNode<ValueType>(Value)){}
template <typename ValueType>
BinHeap<ValueType> :: BinHeap (const BinHeap<ValueType> &h)
{
	_Heap.resize(h._Heap.size(), NULL);
	for (size_t i = 0; i < _Heap.size(); ++i)
		if (h._Heap[i] != NULL)
			_Heap[i] = new BinHeapNode<ValueType>(h._Heap[i]);
}
template <typename ValueType>
BinHeap<ValueType> :: ~BinHeap ()
{
	for (size_t i = 0; i < _Heap.size(); ++i)
		if (_Heap[i] != NULL)
			delete _Heap[i];
}

template <typename ValueType>
void BinHeap<ValueType> :: Clean ()
{
	while (_Heap.size() > 0 && _Heap.back() == NULL)
		_Heap.pop_back();
}

template <typename ValueType>
void BinHeap<ValueType> :: merge (BinHeap<ValueType> &h)
{
	if (h._Heap.size() == 0)
		return;
	if (_Heap.size() == 0)
	{
		_Heap.swap(h._Heap);
		return;
	}

	_Heap.resize(std::max(_Heap.size(), h._Heap.size()) + 1, NULL);
	h._Heap.resize(_Heap.size(), NULL);
	BinHeapNode<ValueType> *nd = NULL, *bf;

	//Делать по количеству не понятно как, ибо пусть,
	//мы даже знаем кол-во 
	//нам придется как-то определять где находятси "живые" кучи
	for (size_t i = 0; i < _Heap.size(); ++i)
	{
		if (_Heap[i] == NULL)
			if (nd == NULL || h._Heap[i] == NULL)
				_Heap[i] = (nd == NULL ? h._Heap[i] : nd), nd = NULL;
			else
				nd = BinHeapNodeMerge(h._Heap[i], nd), _Heap[i] = NULL;
		else
			if (nd != NULL || h._Heap[i] != NULL)
				bf = (nd == NULL ? nd : h._Heap[i]),
				nd = BinHeapNodeMerge((nd == NULL ? h._Heap[i] : nd), _Heap[i]),
				_Heap[i] = bf;
	}
	Clean();
	h._Heap.resize(0);
}

template <typename ValueType>
void BinHeap<ValueType> :: push (ValueType Value)
{
	BinHeap<ValueType> h(Value);
	merge(h);
}

template <typename ValueType>
size_t BinHeap<ValueType> :: FindMinPosition ()const
{
	assert(_Heap.size() > 0 /** "Heap has no elements!" **/ );
	size_t Min = -1;
	for (size_t i = 0; i < _Heap.size(); ++i)
		if (_Heap[i] != NULL)
			if (Min == size_t(-1) || _Heap[i]->_Value < _Heap[Min]->_Value)
				Min = i;
	return Min;
}

template <typename ValueType>
ValueType BinHeap<ValueType> :: top ()const
{
	size_t Min = FindMinPosition();
	return _Heap[Min]->_Value;
}

template <typename ValueType>
void BinHeap<ValueType> :: pop ()
{
	size_t Min = FindMinPosition();
	BinHeap<ValueType> h;
	
	h._Heap.resize(Min, NULL);
	for (size_t i = 0; i < Min; ++i)
		h._Heap[i] = _Heap[Min]->_Son[i];
	_Heap[Min] = NULL;
	merge(h);
}

template <typename ValueType>
bool BinHeap<ValueType> :: empty ()const
{
	return _Heap.size() == 0;
}

const size_t Iterations = 1000 * 1 * 1;

std :: priority_queue <int, std::vector<int>, std::greater<int> > q;

BinHeap <int> h;
std::vector< BinHeap<int>* > vBinHeap;

int main ()
{
	time_t st = std::clock();
	for (size_t i = 0; i < Iterations; ++i)
	{
		int x = rand();
		h.push(x);
		q.push(x);
		assert(h.top() == q.top());
		
		vBinHeap.push_back(new BinHeap<int>(h));
	}

	for (size_t i = 0; i < Iterations - 1; ++i)
	{
		h.pop();
		q.pop();
		assert(h.top() == q.top());
	}
	
	for (size_t i = 0; i < Iterations; ++i)
	{
		int a = rand() % vBinHeap.size(), b = rand() % vBinHeap.size();
		vBinHeap.push_back(new BinHeap<int>(*vBinHeap[a]));
		vBinHeap.push_back(new BinHeap<int>(*vBinHeap[b]));
		vBinHeap[vBinHeap.size() - 2]->merge(*vBinHeap.back());
		delete vBinHeap.back();
		vBinHeap.pop_back();
	}
	
	while (!vBinHeap.empty())
	{
		delete vBinHeap.back();
		vBinHeap.pop_back();
	}
	
	std::cerr << (clock() - st) / double(CLOCKS_PER_SEC);
	
	return 0;
}
