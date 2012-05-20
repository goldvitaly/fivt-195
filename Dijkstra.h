#ifndef DIJKSTRA_H_INCLUDED
#define DIJKSTRA_H_INCLUDED

#include <set>
#include <vector>
#include "Graph.h"

//previous[start] = start
class Path
{
private:
    const std::vector<char>& isReached;
    const std::vector<size_t>& previous;

public:
    size_t to;
    Path(std::vector<char>& _isReached, std::vector<size_t>& _previous):
        isReached(_isReached), previous(_previous) {}
    Path(std::vector<char>& _isReached, std::vector<size_t>& _previous, size_t _to):
        isReached(_isReached), previous(_previous), to(_to) {}
    std::vector<size_t> getPath() const
    {
        std::vector<size_t> res;
		size_t curVertex = to, prevVertex = previous[to];
		while (curVertex != prevVertex)
		{
			res.push_back(curVertex);
			curVertex = prevVertex;
			prevVertex = previous[curVertex];
		}
		res.push_back(curVertex);
		reverse(res.begin(), res.end());
		return res;
    } 
	size_t getPrev()
	{
		to = previous[to];
		return to;
	}
};

// поменять методы, завести структуры соответствующие
template<typename TLength>
class ShortestPathInfo
{
private:
	const std::vector<char> isReached;
	const std::vector<TLength> distances;
	const std::vector<size_t> previous;
public:
	ShortestPathInfo(const std::vector<char>& _isReached, const std::vector<TLength>& _distances, const std::vector<size_t>& _previous) : 
	distances(_distances), isReached(_isReached), previous(_previous){}
	
	TLength getLength(size_t v) const 
	{
		return distances[v];
	}
	
	bool isReach(size_t v) const 
	{
		return isReached[v];
	}

	Path calcPath() const
	{
		Path path(isReached, previous);
		return path;
	}
};

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
        const TCompLength compLength;
        DijkstraStateVertex(size_t _id, const TLength& _curLength, const TCompLength& compLength):
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
        explicit InfoDijkstra(size_t numVertices) : isReached(numVertices, false), curDist(numVertices), previous(numVertices) {}
    };

    class RelaxFunctor : public IncidenceCallBack<TWeight>
    {
    private:
        const TCalcLength& calcLength;
        const TCompLength compLength;
        InfoDijkstra& info;
        Path path;
    public:
        size_t curVertex;
        RelaxFunctor(InfoDijkstra& _info, const TCalcLength& calcLength, const TCompLength& compLength):
            info(_info), calcLength(calcLength), compLength(compLength), path(_info.isReached, _info.previous) {}

        RelaxFunctor(InfoDijkstra& _info, const TCalcLength& calcLength, const TCompLength& compLength, size_t _curVertex):
            info(_info), calcLength(calcLength), compLength(compLength), curVertex(_curVertex), path(_info.isReached, _info.previous) {}
        ~RelaxFunctor() {}

        void operator() (const Edge<TWeight> & edge) override
        {
            TLength newLength;
            path.to = edge.to;
            newLength = calcLength(info.curDist[curVertex], edge.weight, path);
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
    explicit Dijkstra(const Graph<TWeight>& _graph, const TCalcLength& _calcLength = TCalcLength(), const TCompLength& _compLength = TCompLength() ):
        graph(_graph), calcLength(_calcLength), compLength(_compLength) {}

    ShortestPathInfo<TLength> calcDist(size_t from)
    {
        InfoDijkstra info(graph.numVertices());
        info.curDist[from] = TLength();
        info.isReached[from] = true;
        info.previous[from] = from;
        info.queue.insert(DijkstraStateVertex(from, info.curDist[from], compLength));
        RelaxFunctor relaxFunc(info, calcLength, compLength);
        while(!info.queue.empty())
        {
            auto curIterator = info.queue.begin();
            size_t curVertex = curIterator->id;
            info.queue.erase(curIterator);
            relaxFunc.curVertex = curVertex;
            graph.foreachIncidence(curVertex, relaxFunc);
        }
		ShortestPathInfo<TLength> res(info.isReached, info.curDist, info.previous);
		return res;
    }
};

#endif
