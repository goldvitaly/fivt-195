#pragma once

#include "graph.hpp"
#include "path.hpp"

#include <vector>
#include <queue>
#include <memory>

#include <iostream>//DEBUG

namespace graph
{
namespace algo
{

template<typename WeightType>
class Paths
{
	typedef std::vector<std::unique_ptr<Path<WeightType>>> distance_v;

public:
	template<typename PathType>
	void setSize(size_t size);
	Path<WeightType>& operator[](int index);
	const Path<WeightType>& operator[](int index) const;

private:
	distance_v content;
};

template<typename WeightType>
class VertexPriority
{
public:
	explicit VertexPriority(const Paths<WeightType>& dist);
	bool operator()(unsigned v1, unsigned v2) const;

private:
	const Paths<WeightType>& distance;
};

template<typename WeightType>
class DijkstraMaker
{
public:
	explicit DijkstraMaker(const WeightedGraph<WeightType>& gr);
	template<typename PathType>
	Paths<WeightType>& make(unsigned v);

private:
	const WeightedGraph<WeightType>& g;
	std::vector<bool> processed;
	Paths<WeightType> distance;
	std::priority_queue<unsigned, std::vector<unsigned>, VertexPriority<WeightType>> vq;
};

template<typename WeightType>
template<typename PathType>
void Paths<WeightType>::setSize(size_t size)
{
	for(size_t p = 0; p < size; ++p)
		content.push_back(std::unique_ptr<Path<WeightType>>(new PathType()));
}

template<typename WeightType>
Path<WeightType>& Paths<WeightType>::operator[](int index)
{
	return *content[index];
}

template<typename WeightType>
const Path<WeightType>& Paths<WeightType>::operator[](int index) const
{
	return *content[index];
}

template<typename WeightType>
VertexPriority<WeightType>::VertexPriority(const Paths<WeightType>& dist) : distance(dist)
{}

template<typename WeightType>
bool VertexPriority<WeightType>::operator()(unsigned v1, unsigned v2) const
{
	return distance[v2] < distance[v1];
}

template<typename WeightType>
DijkstraMaker<WeightType>::DijkstraMaker(const WeightedGraph<WeightType>& gr) : g(gr), vq(VertexPriority<WeightType>(distance))
{}

template<typename WeightType>
template<typename PathType>
Paths<WeightType>& DijkstraMaker<WeightType>::make(unsigned v)
{
	distance.template setSize<PathType>(g.size());//OMG! O_o
	while(!vq.empty())
		vq.pop();
	for(unsigned v = 0; v < g.size(); ++v)
		vq.push_back(v);
	
	return distance;
}

}
}