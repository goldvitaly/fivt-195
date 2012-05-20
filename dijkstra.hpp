#pragma once

#include "dijkstra_util.hpp"
#include "graph.hpp"
#include "path.hpp"

#include <vector>
#include <queue>
#include <memory>

namespace graph
{
namespace algo
{

template<typename WeightType, typename PathType>
class DijkstraMaker
{
public:
	explicit DijkstraMaker(const WeightedGraph<WeightType>& gr);
	Paths<WeightType, PathType>& make(unsigned v);

private:
	const WeightedGraph<WeightType>& g;
	Paths<WeightType, PathType> distance;
	std::unique_ptr<VertexQueue<WeightType, PathType>> vq;
};

template<typename WeightType, typename PathType>
DijkstraMaker<WeightType, PathType>::DijkstraMaker(const WeightedGraph<WeightType>& gr) :	g(gr),
																							distance(g.size()),
																							vq(new MapVertexQueue<WeightType, PathType>(distance))
{}

template<typename WeightType, typename PathType>
Paths<WeightType, PathType>& DijkstraMaker<WeightType, PathType>::make(unsigned v)
{
	vq->clear();
	distance[v].setZero();
	for(unsigned u = 0; u < g.size(); ++u)
	{
		if(u != v)
			distance[u].setInfinity();
		vq->push(u);
	}
	while(!vq->empty())
	{
		unsigned u = vq->pop();
		for(const auto& p : g.getNode(u))
		{
			if(distance[p.first].worseThan(distance[u], p.second))
			{
				vq->relax(p.first, u, p.second);
			}
		}
	}
	return distance;
}

}
}