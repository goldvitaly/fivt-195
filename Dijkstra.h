#ifndef DIJKSTRA_H_INCLUDED
#define DIJKSTRA_H_INCLUDED

#include <set>
#include <vector>
#include "Graph.h"
#include <memory>

template <typename TWeight>
struct PreviousInfo
{
    size_t v;
    const Edge<TWeight>* edge;
    PreviousInfo(size_t _v, const Edge<TWeight>& _edge): v(_v), edge(&_edge) {}
    PreviousInfo() {}
}; 

//previous[start] = start
template <typename TWeight>
class Path
{
private:
    const std::vector<char>& isReached;
    const std::vector< PreviousInfo<TWeight> >& previous;
    size_t to;
    
public:
    Path(const std::vector<char>& _isReached, const std::vector< PreviousInfo<TWeight> >& _previous, size_t _to):
        isReached(_isReached), previous(_previous), to(_to) {}
    std::vector< PreviousInfo<TWeight> > getPath() const
    {
        std::vector< PreviousInfo<TWeight> > res;
        size_t curVertex = to, prevVertex = previous[to].v;
        while (curVertex != prevVertex)
        {
            res.push_back(previous[curVertex]);
            curVertex = prevVertex;
            prevVertex = previous[curVertex].v;
        }
        reverse(res.begin(), res.end());
        return res;
    } 
    PreviousInfo<TWeight> getPrev()
    {
        PreviousInfo<TWeight> prev = previous[to];
        to = prev.v;
        return std::move(prev);
    }
};

template<typename TWeight, typename TLength>
class ShortestPathInfo
{
private:
    const std::vector<char> isReached;
    const std::vector<TLength> distances;
    const std::vector<PreviousInfo<TWeight> > previous;
public:
    ShortestPathInfo(const std::vector<char>& _isReached, const std::vector<TLength>& _distances, const std::vector<PreviousInfo<TWeight> >& _previous): 
    distances(_distances), isReached(_isReached), previous(_previous){}
    
    TLength getLength(size_t v) const 
    {
        return distances[v];
    }
    
    bool isReach(size_t v) const 
    {
        return bool(isReached[v]);
    }

    Path<TWeight> calcPath(size_t to) const
    {
        Path<TWeight> path(isReached, previous, to);
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
        std::vector< PreviousInfo<TWeight> > previous;
        explicit InfoDijkstra(size_t numVertices) : isReached(numVertices, false), curDist(numVertices), previous(numVertices) {}
    };

    class RelaxFunctor : public IncidenceCallBack<TWeight>
    {
    private:
        const TCalcLength& calcLength;
        const TCompLength compLength;
        InfoDijkstra* info;
        size_t curVertex;
        
    public:
        RelaxFunctor(InfoDijkstra* _info, const TCalcLength& _calcLength, const TCompLength& _compLength, size_t _curVertex):
            info(_info), calcLength(_calcLength), compLength(_compLength), curVertex(_curVertex) {}
        virtual ~RelaxFunctor() {}

        void operator() (const Edge<TWeight>& edge) override
        {
            Path<TWeight> path(info->isReached, info->previous, edge.to);
            TLength newLength = calcLength(info->curDist[curVertex], edge.weight, path);
            if (!info->isReached[edge.to] || compLength(newLength, info->curDist[edge.to]))
            {
                if (info->isReached[edge.to])
                    info->queue.erase(DijkstraStateVertex(edge.to, info->curDist[edge.to], compLength));
                else
                    info->isReached[edge.to] = true;
                info->curDist[edge.to] = newLength;
                info->previous[edge.to] = PreviousInfo<TWeight>(curVertex, edge); //???
                info->queue.insert(DijkstraStateVertex(edge.to, info->curDist[edge.to], compLength));
            }
        }
    };
public:
    explicit Dijkstra(const Graph<TWeight>& _graph, const TCalcLength& _calcLength = TCalcLength(), const TCompLength& _compLength = TCompLength() ):
        graph(_graph), calcLength(_calcLength), compLength(_compLength) {}

    ShortestPathInfo<TWeight, TLength> calcDist(size_t from, const TLength& initialSelfDist)
    {
        InfoDijkstra info(graph.numVertices());
        info.curDist[from] = initialSelfDist;
        info.isReached[from] = true;
        info.previous[from].v = from;
        info.queue.insert(DijkstraStateVertex(from, info.curDist[from], compLength));
        while(!info.queue.empty())
        {
            auto curIterator = info.queue.begin();
            size_t curVertex = curIterator->id;
            info.queue.erase(curIterator);
            RelaxFunctor relaxFunc(&info, calcLength, compLength, curVertex);
            graph.foreachIncidence(curVertex, relaxFunc);
        }
        ShortestPathInfo<TWeight, TLength> res(info.isReached, info.curDist, info.previous);
        return res;
    }
};

#endif
