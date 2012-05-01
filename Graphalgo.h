#ifndef GRAPHALGO_H_INCLUDED	
#define GRAPHALGO_H_INCLUDED

#include <set>
#include <vector>
#include "Graph.h"

typedef std::vector<size_t> Path;

template<typename TLength>
class SumWeightLength
{
public:
	TLength operator() (const TLength& oldLength, const TLength& weight, const Path& path) const 
	{
		return oldLength + weight;
	}
};

template<typename TWeight, typename TLength = TWeight, 
typename TCalcLength = SumWeightLength<TLength> , typename TCompLength = std::less<TLength> >
class Dijkstra
{
private:
	const Graph<TWeight>& graph;
	const TCalcLength calcLength;
	const TCompLength compLength; 
	
	struct DijkstraStateVertex
	{
		size_t id;
		const TLength& curLength;
		const TCompLength& compLength;
		explicit DijkstraStateVertex(): {}
		explicit DijkstraStateVertex(size_t _id, const TLength& _curLength, const TCompLength& compLength): 
		id(_id), curLength(_curLength), compLength(compLength) {}
		bool operator < (const DijkstraStateVertex& other) const
		{
			if (compLength(curLength, other.curLength))
				return true;
			if (compLength(other.curLength, curLength))
				return false;
			return id < other.id;
		}
	};

	struct InfoDijkstra
	{
		std::set<DijkstraStateVertex> queue;
		std::vector<TLength> curDist;
		std::vector<char> isReached;
		std::vector<size_t> previous;
		explicit InfoDijkstra() : {}
		explicit InfoDijkstra(size_t numVertices) : isReached(numVertices, false), curDist(numVertices), previous(numVertices) {}
	};
		
	class RelaxFunctor : public IncidenceCallBack<TWeight>
	{
	private:
		const TCalcLength& calcLength;
		const TCompLength& compLength; 
		InfoDijkstra& info;

	public:
		size_t curVertex;
		explicit RelaxFunctor(): {}
		explicit RelaxFunctor(InfoDijkstra& _info, const TCalcLength& calcLength, const TCompLength& compLength, size_t _curVertex = 0):
		info(_info), calcLength(calcLength), compLength(compLength), curVertex(_curVertex) {}
		~RelaxFunctor() {}
		
		void operator() (const Edge<TWeight> & edge) override
		{
			TLength newLength;
			newLength = calcLength(info.curDist[curVertex], edge.weight, info.previous);
			if (!info.isReached[edge.to] || compLength(newLength, info.curDist[edge.to]))
			{
				if (info.isReached[edge.to])
					info.queue.erase(DijkstraStateVertex(edge.to, info.curDist[edge.to], compLength));
				else
					info.isReached[edge.to] = true;
				info.curDist[edge.to] = newLength;
				info.previous[edge.to] = curVertex;
				info.queue.insert(DijkstraStateVertex(edge.to, info.curDist[edge.to], compLength));	
			}
		}
	};
public:
	explicit Dijkstra(): {}
	explicit Dijkstra(const Graph<TWeight>& _graph, const TCalcLength& _calcLength = TCalcLength(), const TCompLength& _compLength = TCompLength() ): 
	graph(_graph), calcLength(_calcLength), compLength(_compLength){}
	
	void calcDist(size_t from, std::vector<TLength>& outDist, std::vector<char>& outIsReached)
	{
		InfoDijkstra info(graph.numVertices());
		info.curDist[from] = TLength();
		info.isReached[from] = true;
		info.queue.insert(DijkstraStateVertex(from, info.curDist[from], compLength));
		RelaxFunctor relaxFunc(info, calcLength, compLength);
		while(!info.queue.empty())
		{
			std::set< DijkstraStateVertex >::iterator curIterator = info.queue.begin();
			size_t curVertex = curIterator->id;
			info.queue.erase(curIterator);
			relaxFunc.curVertex = curVertex;
			graph.foreachIncidence(curVertex, relaxFunc);
		}

		outDist = info.curDist;
		outIsReached = info.isReached;
	}
};

#endif
