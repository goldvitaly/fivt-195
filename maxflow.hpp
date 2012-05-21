#pragma once

#include "graph.hpp"

namespace graph
{
namespace algo
{

template<typename WeightType>
class MaxFlow
{
public:
	MaxFlow(const WeightedGraph<WeightType>& gr);

private:
	const WeightedGraph<WeightType>& g;
	WeightedGraph<WeightType> remain;
};


template<typename WeightType>
MaxFlow<WeightType>::MaxFlow(const WeightedGraph<WeightType>& gr) : g(gr)
{}


}//namespace algo
}//namespace graph