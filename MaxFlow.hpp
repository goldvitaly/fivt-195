#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP
#include <algorithm>

#include "Edge.hpp"

#include "Incidents.hpp"

#include "Graph.hpp"
#include "ShortestPathsInfo.hpp"
template <typename T>
class Identity {
public:
	typedef T value_type;
	T operator() (const T& value) const {
		return value;
	}
};

template <typename Capacity>
class Min {
public:
	explicit Min(const std::vector<Capacity>& capacities):capacities(capacities){}
	Capacity operator()(Capacity oldMin, size_t newEdgeID, const Path<size_t>&) const {
		return std::min(oldMin, capacities[newEdgeID]);
	}
private:
	const std::vector<Capacity>& capacities;
};

template<typename Weight, typename GetCapacity = Identity<Weight> >
class MaxFlow {
	typedef typename GetCapacity::value_type Capacity;
public:
	explicit MaxFlow(const Graph<Weight>& g, GetCapacity getCapacity = GetCapacity()){
		for(size_t i = 0; i < g.size(); ++i){
			graph.addVertex(std::unique_ptr<Incidents<size_t> > (new VectorIncidents<size_t>()));
		}
		for(size_t i = 0; i < g.size(); ++i){
			for(const Edge<Weight>& edge: g.getIncidents(i)){
				graph.addEdge(i, edge.id, edges.size());
				edges.push_back(getCapacity(edge.weight));
				graph.addEdge(edge.id, i, edges.size());
				edges.push_back(Capacity(0));
			}
		}
	}
	
	Capacity maxFlow(size_t from, size_t to){
		Capacity flow(0);
		while(true){
			ShortestPaths<size_t, Capacity, Min<Capacity>, std::greater<Capacity> > sp(graph, std::numeric_limits<Capacity>::max(), Min<Capacity>(edges));
			ShortestPathsInfo<Capacity, size_t> result = sp.calculate(from);
			if(!result.length(to))
				break;
			Capacity addFlow = *result.length(to);
			if(addFlow == Capacity(0))
				break;
			std::vector<typename Incidents<size_t>::Iterator> path = result.path(to);
			for(const Incidents<size_t>::Iterator& edge: path){
				size_t curEdgeID = (*edge).weight;
				size_t pairedEdgeID = pairedEdge(curEdgeID);
				edges[curEdgeID] -= addFlow;
				edges[pairedEdgeID] += addFlow;
			}
			flow += addFlow;
		}
		return flow;
	}
	
private:
	size_t pairedEdge(size_t edgeID){
		return edgeID ^ 1;
	}
	std::vector<Capacity> edges;
	Graph<size_t> graph;
};

#endif /* MAXFLOW_HPP */

