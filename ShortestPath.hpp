#ifndef SHORTESTPATH_HPP
#define SHORTESTPATH_HPP
#include <boost/optional.hpp>
#include <set>
#include <vector>

#include "ShortestPathsInfo.hpp"

template<typename Weight, typename Length = Weight, typename CalcLength = std::plus<Weight> >
class ShortestPath{
public:
	explicit ShortestPath(const Graph<Weight>& graph, const CalcLength& calcLength = CalcLength())
		:graph(graph), calcLength(calcLength){}
	
	ShortestPathsInfo<Length> calculate(size_t from) const {
		std::set<State> queue;
		std::vector<boost::optional<Length> > curLen(graph.size());
		std::vector<boost::optional<size_t> > previous(graph.size());
		curLen[from] = Length();
		queue.insert(State(from, *curLen[from]));
		while(!queue.empty()){
			const State& curState = *queue.begin();
			for(const Vertex<Weight>& next: graph.getIncidents(curState.id)){
				Length newLen = calcLength(*curLen[curState.id], next.weight);
				if(curLen[next.id]){
					if(newLen < *curLen[next.id]){
						queue.erase(State(next.id, *curLen[next.id]));
						curLen[next.id] = newLen;
						previous[next.id] = curState.id;
						queue.insert(State(next.id, *curLen[next.id]));
					}
				}
				else {
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
		bool operator < (const State& other) const {
			return curLen < other.curLen || (curLen == other.curLen && id < other.id);
		}
	};
	
	const Graph<Weight>& graph;
	CalcLength calcLength;
};

#endif /* SHORTESTPATH_HPP */

