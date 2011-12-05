#ifndef HNODE_H
#define HNODE_H

#include <cassert>
#include <vector>
#include <iostream>

/**
 * Больше пока не буду выпендриваться и писать комментарии на английском.
 * 
 * БИНОМИАЛЬНАЯ КУЧА
 * 
 * hnode - вершина биномиального дерева.
 * 
 * value - значение, хранящееся в вершине.
 * child - вектор указателей на детей.
 * 
 * Конструктор по умолчанию не делает ничего, деструктор по умолчанию удаляет себя и всех детей, конструктор от значения сразу записывает значение в вершину.
 * 
 * get_child возвращает ссылку на вектор детей (к нему нельзя обратиться в явном виде, поскольку он приватный)
 * cleanup очищает вектор детей, не удаляя при этом самих детей. Необходимо для аккуратного удаления самой вершины, если нужно сохранить детей.
 * 
 * top - минимальный элемент, лежит в корне. Сложность - O(1)
 * rank - порядок дерева. Равен количеству детей. Сложность - O(1)
 * size - количество элементов в дереве. Сложность - O(1).
 * 
 * out - рекурсивная функция вывода дерева
 * 
 * merge принимает в качестве параметра второе дерево, сливает его с данным и возвращает результат слияния.
 * TODO: переписать merge, чтобы он "приклеивал" новое дерево к текущему, а не создавал новый результат.
 * 
 */

template<typename T> 
class hnode
{
	private:
		T value;
		std :: vector<hnode<T>*> child;
	public:
	static int count;
		hnode();
		~hnode();
		hnode(T);
		
		std :: vector<hnode<T>*>& get_child();
		void cleanup();
		
		T top();
		size_t rank();
		size_t size();
		
		void out();
		
		hnode<T> *merge (hnode<T> *);
};
template<typename T>
hnode<T> :: hnode()
{
	count++;
}
template<typename T>
hnode<T> :: ~hnode()
{
	count--;
	for (typename std :: vector<hnode<T> *> :: iterator it = child.begin(); it != child.end(); it++)
		delete *it;
}
template<typename T>
hnode<T> :: hnode(T new_value)
{
	count++;
	value = new_value;
}
template<typename T>
T hnode<T> :: top()
{
	return value;
}
template<typename T>
size_t hnode<T> :: rank()
{
	return child.size();
}
template<typename T>
size_t hnode<T> :: size()
{
	return 1 << rank();
}

template<typename T>
hnode<T> * hnode<T> :: merge (hnode<T> *foo)
{
	assert(rank() == foo->rank());
	if (top() <= foo->top())
	{
		child.push_back(foo);
		return this;
	}
	else
		return foo->merge(this);
}

template<typename T>
void hnode<T> :: out()
{
	std :: cout << value << " ";
	for (typename std :: vector<hnode<T> *> :: iterator it = child.begin(); it != child.end(); it++)
		(*it)->out();
}

template<typename T>
int hnode<T> :: count = 0;

template<typename T>
void hnode<T> :: cleanup()
{
	child.clear();
}
template<typename T>
std :: vector<hnode<T>*>& hnode<T> :: get_child()
{
	return child;
}
#endif
