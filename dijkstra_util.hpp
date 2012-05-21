#pragma once

#include "path.hpp"

#include <set>

namespace graph
{

//where is my std::priority_queue with decrease_key()?
template<typename PathType>
class VertexQueue
{
public:
	virtual ~VertexQueue();
	virtual unsigned pop() = 0;
	virtual bool empty() = 0;
	virtual void remove(unsigned v, PathType path) = 0;
	virtual void push(unsigned v, PathType path) = 0;
	virtual void clear() = 0;
};

template<typename PathType>
class SetVertexQueue : public VertexQueue<PathType>
{
public:
	virtual ~SetVertexQueue();
	virtual unsigned pop();
	virtual bool empty();
	virtual void remove(unsigned v, PathType path);
	virtual void push(unsigned v, PathType path);
	virtual void clear();

private:
	std::set<std::pair<PathType, unsigned>> heap;

};

template<typename PathType>
VertexQueue<PathType>::~VertexQueue() {}

template<typename PathType>
SetVertexQueue<PathType>::~SetVertexQueue()
{}

template<typename PathType>
unsigned SetVertexQueue<PathType>::pop()
{
	unsigned result = (*heap.begin()).second;
	heap.erase(heap.begin());
	return result;
}

template<typename PathType>
bool SetVertexQueue<PathType>::empty()
{
	return heap.empty();
}

template<typename PathType>
void SetVertexQueue<PathType>::remove(unsigned v, PathType path)
{
	heap.erase(heap.find(std::make_pair(path, v)));
}

template<typename PathType>
void SetVertexQueue<PathType>::push(unsigned v, PathType path)
{
	heap.insert(std::make_pair(path, v));
}

template<typename PathType>
void SetVertexQueue<PathType>::clear()
{
	heap.clear();
}

}//namespace graph