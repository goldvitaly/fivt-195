#pragma once

#include "graph.hpp"

#include <vector>
#include <queue>

namespace graph
{
namespace algo
{

template<typename WeightType>
class DijkstraMaker
{
public:
	explicit DijkstraMaker(const WeightedGraph<WeightType>& gr);

	std::vector<WeightType> make(unsigned v);
private:
	const WeightedGraph<WeightType>& g;
};

template<typename WeightType>
DijkstraMaker<WeightType>::DijkstraMaker(const WeightedGraph<WeightType>& gr) : g(gr) {}

template<typename WeightType>
std::vector<WeightType> DijkstraMaker<WeightType>::make(unsigned v)
{
	std::priority_queue<unsigned> vq;
	//...
}
	
}
}