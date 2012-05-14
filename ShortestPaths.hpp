#ifndef SHORTESTPATH_HPP
#define SHORTESTPATH_HPP
#include "Graph.hpp"
#include "Path.hpp"
#include "ShortestPathsInfo.hpp"
#include <boost/optional.hpp>
#include <set>
#include <vector>

template <typename Weight>
class SumShortestPathsFunctor{
public:
	Weight operator() (const Weight& oldLen, const Weight& weight, const Path<Weight>&) const {
		return oldLen + weight;
	}
};

template<typename Weight, typename Length = Weight, typename CalcLength = SumShortestPathsFunctor<Length> , typename CompLength = std::less<Length> >
class ShortestPaths{
public: 
	explicit ShortestPaths(const Graph<Weight>& graph, const CalcLength& calcLength = CalcLength(), const CompLength& compLength = CompLength())
		:graph(graph), calcLength(calcLength), compLength(compLength){}
	
	ShortestPathsInfo<Length,Weight> calculate(size_t from){
		std::set<State, StateComparator> queue(compLength);
		curLen.assign(graph.size(), boost::optional<Length>());
		previous.assign(graph.size(), boost::optional<size_t>());
		for(size_t i=0;i<graph.size();++i){
			lastEdge.push_back(std::move(Iterator()));
		}
		curLen[from] = Length();
		queue.insert(State(from, *curLen[from]));
		while(!queue.empty()){
			auto firstIterator = queue.begin();
			const State& curState = *firstIterator;
			for(Iterator it = graph.getIncidents(curState.id).begin();it != graph.getIncidents(curState.id).end(); ++it){
				const Edge<Weight>& next = *it;
				Length newLen = calcLength(*curLen[curState.id], next.weight, Path<Weight>(previous, next.id, lastEdge));
				if(!curLen[next.id] || compLength(newLen , *curLen[next.id])){
					if(curLen[next.id])
						queue.erase(State(next.id, *curLen[next.id]));
					curLen[next.id] = newLen;
					previous[next.id] = curState.id;
					lastEdge[next.id] = it.copy();
					queue.insert(State(next.id, *curLen[next.id]));
				}
			}
			queue.erase(firstIterator);
		}
		return ShortestPathsInfo<Length, Weight>(curLen, previous, lastEdge);
	}
private:
	struct State{
		size_t id;
		const Length& curLen;
		State(size_t id, const Length& curLen):id(id), curLen(curLen){}
	};
	struct StateComparator{
		StateComparator(const CompLength& compLength):
			compLength(compLength){}
		bool operator ()(const State& first, const State& second) const {
			if(compLength(first.curLen, second.curLen))
				return true;
			if(compLength(second.curLen, first.curLen))
				return false;
			return first.id < second.id;
		}
	private:
		CompLength compLength;
	};	
	
	const Graph<Weight>& graph;
	CalcLength calcLength;
	CompLength compLength;
	
	std::vector<boost::optional<Length> > curLen;
	std::vector<boost::optional<size_t> > previous;
	typedef typename Incidents<Weight>::Iterator Iterator;
	std::vector<Iterator> lastEdge;
	
};

#endif /* SHORTESTPATH_HPP */

