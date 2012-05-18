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
class DijkstraMaker
{
public:
	explicit DijkstraMaker(const WeightedGraph<WeightType>& gr);

	std::vector<std::unique_ptr<Path<WeightType>>>& make(unsigned v);
private:
	const WeightedGraph<WeightType>& g;
	std::priority_queue<unsigned> vq;
	std::vector<std::unique_ptr<Path<WeightType>>> distance;
	std::vector<bool> processed;

	void reset();
};

template<typename WeightType>
DijkstraMaker<WeightType>::DijkstraMaker(const WeightedGraph<WeightType>& gr) : g(gr)
{
	vq = std::priority_queue<unsigned>();
}

template<typename WeightType>
std::vector<std::unique_ptr<Path<WeightType>>>& DijkstraMaker<WeightType>::make(unsigned v)
{
	reset();
	return distance;
}

template<typename WeightType>
void DijkstraMaker<WeightType>::reset()
{
	
}

}
}