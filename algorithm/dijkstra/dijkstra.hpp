#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "../../Graph/Graph.hpp"

#include <algorithm>

namespace DijkstraShortestPaths
{
template<typename TEdge, typename TDist, typename TWeight>
class FunctionTypes
{
private:
    FunctionTypes() = delete;

public:
    typedef TWeight (*WeightExtractor)(TEdge);
    typedef size_t (*IncidentExtractor)(TEdge);
    typedef bool (*Less)(TDist, TDist);
    typedef TWeight (*Plus)(TDist, TWeight);
};

template<typename TDist>
class VertexInfo
{
public:
    VertexInfo(bool reach_, TDist dist_): reach(reach_), dist(dist_) {}
    VertexInfo() {};

public:
    bool reach;
    size_t dist;
};

template<typename TEdge,
         typename TDist = int,
         typename TWeight = TDist,
         typename Plus = std::plus<TDist>,
         typename Less = std::less<TDist>,
         typename WeightExtractor = typename FunctionTypes<TEdge, TDist, TWeight>::WeightExtractor,
         typename IncidentExtractor = typename FunctionTypes<TEdge, TDist, TWeight>::IncidentExtractor>
class SPFinder
{
public:
    SPFinder(const graph::Graph<TEdge> &graph_, 
             Plus plus_, Less less_,
             WeightExtractor wExtr_,
             IncidentExtractor iExtr_,
             TDist zero_ = 0):
                graph(graph_),
                plus(plus_), less(less_),
                wExtr(wExtr_),
                iExtr(iExtr_),
                zero(zero_) {}
    SPFinder(const graph::Graph<TEdge> &graph_,
             WeightExtractor wExtr_,
             IncidentExtractor iExtr_,
             TDist zero_ = 0):
                graph(graph_),
                plus(Plus()), less(Less()),
                wExtr(wExtr_),
                iExtr(iExtr_),
                zero(zero_) {}

    void calcSPDense(size_t src)
    {
        std::vector<char> inQueue(graph.size(), false);
        anc.assign(graph.size(), -1);
        dist.assign(graph.size(), zero);
        reach.assign(graph.size(), false);
        incomingEdge.assign(graph.size(), TEdge()); // здесь должен быть boost::optional,
                                                    // но я пока не умею им пользоваться

        inQueue[src] = true;

        while (true)
        {
            size_t curv = NO_VALUE;
            for (size_t i = 0; i < graph.size(); i++)
                if (!reach[i] && inQueue[i] && (curv == NO_VALUE || less(dist[i], dist[curv])))
                    curv = i;
            if (curv == NO_VALUE)
                break;
            
            reach[curv] = true;
            inQueue[curv] = false;

            for (auto edge: graph[curv])
            {
                size_t dest = iExtr(edge);
                if (dest == NO_VALUE || reach[dest])
                    continue;
                TDist weight = wExtr(edge);
                size_t newDist = plus(dist[curv], weight);
                if (!inQueue[dest] || less(newDist, dist[dest]))
                {
                    inQueue[dest] = true;
                    dist[dest] = newDist;
                    anc[dest] = curv;
                    incomingEdge[dest] = edge;
                }
            }
        }

    }
    VertexInfo<TDist> getDist(size_t dest)
    {
        return VertexInfo<TDist>(reach[dest], dist[dest]);
    }
    std::vector<size_t> getPath(size_t dest)
    {
        std::vector<size_t> res;
        if (!reach[dest])
            return res;
        do
        {
            res.push_back(dest);
            dest = anc[dest];
        }
        while (dest != NO_VALUE);
        std::reverse(res.begin(), res.end());
        return res;
    }
    std::vector<TEdge> getEdgePath(size_t dest)
    {
        std::vector<TEdge> res;
        if (!reach[dest])
            return res;
        while (dest != NO_VALUE)
        {
            res.push_back(incomingEdge[dest]);
            dest = anc[dest];
        }
        std::reverse(res.begin(), res.end());
        return res;
    }

private: // user data
    const graph::Graph<TEdge>& graph;
    Plus plus;
    Less less;
    WeightExtractor wExtr;
    IncidentExtractor iExtr;
    const TDist zero;

private: // algo stuff
    std::vector<size_t> anc;
    std::vector<TEdge> incomingEdge;
    std::vector<TDist> dist;
    std::vector<char> reach;

    static const size_t NO_VALUE = -1;
};
} // namespace DijkstraShortestPaths

/**
 * Какие инструменты нужны со стороны пользователя?
 * 
 * 1) тип ребра TEdge - передается через граф
 * 2) тип расстояния TDist
 * 3) тип веса в ребре
 * 3, 4) прибавлятор и сравнятор Plus/Less
 * 5) WeightExtractor - функтор/функция, которая позволяет вычленить
 * из ребра вес TWeight, который можно прибавить к расстоянию. 
 * 
 * Как делать экстрактор - функцией или экземпляром класса?
    typedef int TBase;
 */


#endif
