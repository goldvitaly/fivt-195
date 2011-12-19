#ifndef BHEAP_H
#define BHEAP_H

#include <vector>
#include <iostream>
#include "hnode.h"

/** БИНОМИАЛЬНАЯ КУЧА
 * bheap - собственно носитель кучи. Хранит несколько биномиальных деревьев типа hnode и позволяет производить с ними различные операции.
 * 
 * heaps - вектор указателей на деревья, в которых хранятся элементы
 * _size - размер дерева
 * 
 * Стандартный конструктор только инициализирует размер, стандартный деструктор не делает ничего.
 * Конструктор от вектора указателей на деревья создает кучу, хранящую все деревья в векторе. При этом считается, что каждое дерево в векторе не пусто
 * и i-е дерево имеет порядок ровно i.
 * 
 * merge принимает в качестве параметра вторую кучу. После выполнения в первой оказывается результат слияния двух куч. Сложность - O(log N).
 * 
 * size - возвращает размер. Сложность - O(1)
 * empty - возвращает true тогда и только тогда, когда куча пуста. Сложность - O(1)
 * 
 * top - возвращает минимальный элемент кучи. Сложность - O(log N). Реализовано линейным проходом по корням всех деревьев.
 * pop - возвращает минимальный элемент кучи и удаляет его. Сложность - O(log N).
 * push - добавляет элемент в кучу, даже в том случае, если элемент с таким значением там уже есть. Сложность - O(log N).
 * cleanup - очищает вектор деревьев, не удаляя сами деревья
 * 
 * out и pretty_out - поэлементный вывод кучи. Сложность - O(N).
 */

template<typename T>
class bheap
{
	private:
		std :: vector<hnode<T>*> heaps;
		size_t _size;
		explicit bheap(std :: vector<hnode<T>*>&);
	public:
		bheap();
		~bheap();
		
		void merge(bheap&);
		
		size_t size() const;
		bool empty() const;
		
		const T& top() const;
		void push(T);
		T pop();
		void cleanup();
		
		void out(std :: ostream&) const;
		void pretty_out(std :: ostream&) const;
};

template<typename T>
bheap<T> :: bheap()
{
	_size = 0;
	heaps.resize(0);
}
template<typename T>
bheap<T> :: ~bheap()
{
}
template<typename T>
bheap<T> :: bheap(std :: vector<hnode<T> *>& foo)
{
	_size = (1 << foo.size()) - 1;
	heaps = foo;
}

template<typename T>
void bheap<T> :: out(std :: ostream& cout) const
{
	for (typename std :: vector<hnode<T>*> :: const_iterator it = heaps.begin(); it != heaps.end(); it++)
		if (*it != NULL)
			(*it)->out(cout);
}
template<typename T>
void bheap<T> :: pretty_out(std :: ostream& cout) const
{
	for (typename std :: vector<hnode<T>*> :: const_iterator it = heaps.begin(); it != heaps.end(); it++)
	{
		if (*it != NULL)
			(*it)->out(cout);
		cout << "| ";
	}
	cout << std :: endl;
}


template<typename T>
void bheap<T> :: merge (bheap &foo) // this = merge(this, foo)
{
	heaps.resize(std :: max(heaps.size(), foo.heaps.size()));
	_size += foo._size;
	hnode<T>* carry = NULL;
	for (size_t i = 0; i < heaps.size(); i++)
	{
		hnode<T>* heap1 = heaps[i], *heap2 = (i < foo.heaps.size() ? foo.heaps[i] : NULL);
		if (heap1 == NULL)
			swap(heap1, heap2); //now heap1 >= heap2;
		if (heap1 == NULL)
		{
			heaps[i] = carry;
			carry = NULL;
		}
		else if (heap2 == NULL)
		{
			if (carry == NULL)
				heaps[i] = heap1;
			else
			{
				heaps[i] = NULL;
				carry = carry->merge(heap1);
			}
		}
		else
		{
			heaps[i] = heap1->merge(heap2);
			swap(heaps[i], carry);
		}
		/*if ( (heaps[i] == NULL) && (i >= foo.heaps.size() || foo.heaps[i] == NULL)) //both are empty
		{
			heaps[i] = carry;
			carry = NULL;
		}
		else if ( (heaps[i] == NULL) ) //first is empty
		{
			if (carry == NULL)
				heaps[i] = foo.heaps[i];
			else
			{
				heaps[i] = NULL;
				carry = carry->merge(foo.heaps[i]);
			}
		}
		else if ( (i >= foo.heaps.size() || foo.heaps[i] == NULL) ) //second is empty
		{
			if (carry != NULL)
			{
				carry = carry->merge(heaps[i]);
				heaps[i] = NULL;
			}
		}
		else //none is empty
		{
			hnode<T>* carry1 = heaps[i]->merge(foo.heaps[i]);
			heaps[i] = carry;
			carry = carry1;
		}*/
	}
	if (carry != NULL)
		heaps.push_back(carry);
	foo.cleanup();
}

template<typename T>
size_t bheap<T> :: size() const
{
	return _size;
}
template<typename T>
bool bheap<T> :: empty() const
{
	return _size == 0;
}

template<typename T>
const T& bheap<T> :: top() const
{
	assert(_size != 0);
	const T* mn = &(heaps.back()->top());
	for (typename std :: vector<hnode<T> *> :: const_iterator it = heaps.begin(); it != heaps.end(); it++)
		if (*it != NULL && (*it)->top() < *mn)
			mn = &((*it)->top());
	return *mn;
}
template<typename T>
void bheap<T> :: push(T new_value)
{
	bheap<T> t;
	t.heaps.push_back(new hnode<T>(new_value));
	t._size++;
	
	merge(t);
}
template<typename T>
T bheap<T> :: pop()
{
	// не const T& pop(), поскольку возвращаемое значение больше не хранится в дереве и возвращать ссылку не на что.
	assert(_size != 0);
	int mnpos = heaps.size() - 1;
	T mn = heaps.back()->top();
	for (typename std :: vector<hnode<T> *> :: iterator it = heaps.begin(); it != heaps.end(); it++)
	{
		if ((*it) != NULL && (*it)->top() < mn)
		{
			mn = (*it)->top();
			mnpos = it - heaps.begin();
		}
	}
	
	bheap<T> t(heaps[mnpos]->get_child());
	heaps[mnpos]->cleanup();
	delete heaps[mnpos];
	heaps[mnpos] = NULL;
	if (mnpos == (int)heaps.size() - 1)
		heaps.pop_back();
	_size -= t.size() + 1;
	merge(t);
	return mn;
}
template<typename T>
void bheap<T> :: cleanup()
{
	heaps.clear();
	_size = 0;
}
#endif
