#pragma once

#include "path.hpp"

#include <set>

namespace graph
{

//where is my std::priority_queue with decrease_key()?
template<typename WeightType, typename PathType>
class VertexQueue
{
public:
	VertexQueue(Paths<WeightType, PathType>& paths);
	virtual ~VertexQueue();
	virtual unsigned pop() = 0;
	virtual bool empty() = 0;
	virtual void relax(unsigned v, unsigned u, WeightType edge) = 0;
	virtual void push(unsigned v) = 0;
	virtual void clear() = 0;

protected:
	Paths<WeightType, PathType>& paths;
};

template<typename WeightType, typename PathType>
class MapVertexQueue : public VertexQueue<WeightType, PathType>
{
public:
	MapVertexQueue(Paths<WeightType, PathType>& paths);
	virtual unsigned pop();
	virtual bool empty();
	virtual void relax(unsigned v, unsigned u, WeightType edge);
	virtual void push(unsigned v);
	virtual void clear();

private:
	std::set<std::pair<PathType, unsigned>> heap;

};

template<typename WeightType, typename PathType>
VertexQueue<WeightType, PathType>::VertexQueue(Paths<WeightType, PathType>& paths) : paths(paths)
{}

template<typename WeightType, typename PathType>
VertexQueue<WeightType, PathType>::~VertexQueue() {}

template<typename WeightType, typename PathType>
MapVertexQueue<WeightType, PathType>::MapVertexQueue(Paths<WeightType, PathType>& paths) : VertexQueue<WeightType, PathType>(paths)
{}

template<typename WeightType, typename PathType>
unsigned MapVertexQueue<WeightType, PathType>::pop()
{
	unsigned result = (*heap.begin()).second;
	heap.erase(heap.begin());
	return result;
}

template<typename WeightType, typename PathType>
bool MapVertexQueue<WeightType, PathType>::empty()
{
	return heap.empty();
}

template<typename WeightType, typename PathType>
void MapVertexQueue<WeightType, PathType>::relax(unsigned v, unsigned u, WeightType edge)
{
	heap.erase(heap.find(std::make_pair((this->paths).at(v), v)));
	(this->paths)[v] = (this->paths)[u];
	(this->paths)[v].addEdge(v, edge);
	push(v);
}

template<typename WeightType, typename PathType>
void MapVertexQueue<WeightType, PathType>::push(unsigned v)
{
	heap.insert(std::make_pair((this->paths).at(v), v));
}

template<typename WeightType, typename PathType>
void MapVertexQueue<WeightType, PathType>::clear()
{
	heap.clear();
}

}