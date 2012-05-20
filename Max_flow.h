#ifndef MAX_FLOW_H_INCLUDED
#define MAX_FLOW_H_INCLUDED

#include "Dijkstra.h"

template <typename ValueType>
class Identity
{
public:
    ValueType operator() (const ValueType& value) const
    {
        return value;
    }
};

template <typename TWeight, typename TCapacity>
class Min
{
private:
    const std::vector<TCapacity>& capacities;
public:
    explicit Min(const std::vector<TCapacity>& _capacities): capacities(_capacities) {}
    TCapacity operator() (const TCapacity& oldMin, size_t newEdgeID, const Path<TWeight>&) const
    {
        return std::min(oldMin, capacities[newEdgeID]);
    }
};

template<typename TCapacity>
class MaxFlowInfo
{
private:
    TCapacity flow;
public:
    MaxFlowInfo(): flow(TCapacity(0)) {}
    TCapacity valueFlow() const
    {
        return flow;
    }
    void addFlow(const TCapacity& x)
    {
        flow += x;
    }
};


template<typename TWeight, typename TCapacity = TWeight, typename GetCapacity = Identity<TWeight> >
class MaxFlow
{
private:
    typedef size_t WeightForFlow;
    Graph<WeightForFlow> graph;
    const Graph<TWeight>& originalGraph;
    std::vector<TCapacity> edges;

    class CreatingNewGraph : public IncidenceCallBack<TWeight>
    {
    private:
        Graph<WeightForFlow>& graph;
        const GetCapacity& getCapacity;
        std::vector<TCapacity>* edges;
        size_t from;
        
    public:
        CreatingNewGraph(Graph<WeightForFlow>& _graph, const GetCapacity& _getCapacity, std::vector<TCapacity>* _edges, size_t _from):
            graph(_graph), getCapacity(_getCapacity), edges(_edges), from(_from) {}

        virtual ~CreatingNewGraph() {}

        void operator() (const Edge<TWeight>& edge) override
        {
            graph.addEdge(from, edge.to, WeightForFlow(edges->size()));
            graph.addEdge(edge.to, from, WeightForFlow(edges->size() + 1));
            edges->push_back(getCapacity(edge.weight));
            edges->push_back(TCapacity(0));
        }
    };

    size_t pairedEdge(size_t edgeID) const
    {
        return edgeID | 1;
    }

public:
    explicit MaxFlow(const Graph<TWeight>& _graph, GetCapacity getCapacity = GetCapacity() ): originalGraph(_graph)
    {
        graph.resize(_graph.numVertices());
        for (size_t i = 0; i < _graph.numVertices(); ++i)
        {
            CreatingNewGraph creatingNewGraph(graph, getCapacity, &edges, i);
            _graph.foreachIncidence(i, creatingNewGraph);
        }
    }

    MaxFlowInfo<TCapacity> calcFlow(size_t from, size_t to)
    {
        MaxFlowInfo<TCapacity> curFlow;
        while (true)
        {
            Dijkstra<WeightForFlow, TCapacity, Min<WeightForFlow, TCapacity>, std::greater<TCapacity> > sp(graph, Min<WeightForFlow, TCapacity>(edges));
            ShortestPathInfo<WeightForFlow, TCapacity> resultShortestPaths = sp.calcDist(from, std::numeric_limits<TCapacity>::max());
            if (!resultShortestPaths.isReach(to))
                break;
            TCapacity addFlow = resultShortestPaths.getLength(to);
            if (addFlow == TCapacity(0))
                break;
            Path<WeightForFlow> path = resultShortestPaths.calcPath(to);
            std::vector< PreviousInfo<WeightForFlow> > p = path.getPath();
            for (size_t i = 0; i < p.size(); i++)
            {
                size_t curEdgeID = p[i].edge->weight;
                size_t pairedEdgeID = pairedEdge(curEdgeID);
                edges[curEdgeID] -= addFlow;
                edges[pairedEdgeID] += addFlow;
            }
            curFlow.addFlow(addFlow);
        }
        return curFlow;
    }
};

#endif
