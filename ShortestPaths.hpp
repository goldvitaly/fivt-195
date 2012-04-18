#ifndef SHORTESTPATH_HPP
#define SHORTESTPATH_HPP
#include "Path.hpp"
#include "ShortestPathsInfo.hpp"
#include <boost/optional.hpp>
#include <set>
#include <vector>

template <typename Weight>
class SumShortestPathsFunctor{
public:
	Weight operator() (const Weight& oldLen, const Weight& weight, const Path&){
		return oldLen + weight;
	}
};

template<typename Weight, typename Length = Weight, typename CalcLength = std::plus<Weight> , typename CompLength = std::less<Length> >
class ShortestPaths{
public: 
	explicit ShortestPaths(const Graph<Weight>& graph, const CalcLength& calcLength = CalcLength(), const CompLength& compLength = CompLength())
		:graph(graph), calcLength(calcLength), compLength(compLength){}
	
	ShortestPathsInfo<Length> calculate(size_t from) const {
		std::set<State, StateComparator> queue(compLength);
		std::vector<boost::optional<Length> > curLen(graph.size());
		std::vector<boost::optional<size_t> > previous(graph.size());
		curLen[from] = Length();
		queue.insert(State(from, *curLen[from]));
		while(!queue.empty()){
			const State& curState = *queue.begin();
			for(const Vertex<Weight>& next: graph.getIncidents(curState.id)){
				Length newLen = calcLength(*curLen[curState.id], next.weight);
				if(!curLen[next.id] || compLength(newLen , *curLen[next.id])){
					if(curLen[next.id])
						queue.erase(State(next.id, *curLen[next.id]));
					curLen[next.id] = newLen;
					previous[next.id] = curState.id;
					queue.insert(State(next.id, *curLen[next.id]));
				}
			}
			queue.erase(*queue.begin());
		}
		return ShortestPathsInfo<Length>(curLen, previous);
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
};

#endif /* SHORTESTPATH_HPP */

