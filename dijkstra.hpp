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
	Paths<WeightType, PathType> paths;
	std::unique_ptr<VertexQueue<WeightType, PathType>> vq;
};

template<typename WeightType, typename PathType>
DijkstraMaker<WeightType, PathType>::DijkstraMaker(const WeightedGraph<WeightType>& gr) :	g(gr),
																							paths(g.size()),
																							vq(new MapVertexQueue<WeightType, PathType>(paths))
{}

template<typename WeightType, typename PathType>
Paths<WeightType, PathType>& DijkstraMaker<WeightType, PathType>::make(unsigned v)
{
	vq->clear();
	paths[v].setZero();
	for(unsigned u = 0; u < g.size(); ++u)
	{
		if(u != v)
			paths[u].setInfinity();
		vq->push(u);
	}
	while(!vq->empty())
	{
		unsigned u = vq->pop();
		for(const auto& p : g.getNode(u))
		{
			if(paths[p.first].worseThan(paths[u], p.second))
			{
				vq->relax(p.first, u, p.second);
			}
		}
	}
	return paths;
}

}
}