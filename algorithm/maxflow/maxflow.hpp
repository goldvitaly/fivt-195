#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP

#include "../../Graph/Graph.hpp"
#include "../dijkstra/dijkstra.hpp"
#include "../../incidences/VectorIncidence.hpp"

// написано на коленке, пока нет поддержки пользовательских величин пропускной способности
class MaxflowFinder
{
public:
    MaxflowFinder(const graph::Graph<std::pair<size_t, int> >& network_):
        network(network_)
    {
        for (size_t i = 0; i < network.size(); i++)
            graph.addVertex(graph::Graph<size_t>::IncidencePtr(new VectorIncidence<size_t>()));
        int edgesNum = 0;
        for (size_t i = 0; i < network.size(); i++) for (auto edge: network[i])
        {
            int from = i, to = edge.first, capacity_ = edge.second;
            graph.addEdge(from, edgesNum);
            graph.addEdge(to, edgesNum);

            incident.push_back(to);
            capacity.push_back(capacity_);
            rev.push_back(edgesNum+1);

            incident.push_back(from);
            capacity.push_back(0);
            rev.push_back(edgesNum);
            
            edgesNum += 2;
        }
    }

    int maxFlow(size_t source_, size_t sink_)
    {
        source = source_;
        sink = sink_;

        flow.assign(flow.size(), 0);
        int res = 0;

        while (int curFlow = applyAugPath(findShortestPath()))
            res += curFlow;
        return res;
    }

private:
    enum ExtractorType {E_Incident, E_Weight};
    class Extractor
    {
    public:
        Extractor (const std::vector<int>& capacity_,
                   const std::vector<int>& flow_,
                   const std::vector<size_t>& incident_,
                   ExtractorType type_):
                   capacity(capacity_), flow(flow_),
                   incident(incident_), type(type_) {}

        int operator() (size_t edge)
        {
            if (type == E_Incident)
                return capacity[edge] > flow[edge] ? incident[edge] : -1;
            return capacity[edge] - flow[edge];
        }

    private:
        const std::vector<int>& capacity;
        const std::vector<int>& flow;
        const std::vector<size_t>& incident;
        ExtractorType type;
    };
    
    // returns value of a minimum edge on the path
    // here path is a sequence of edges, NOT VERTICES!
    int applyAugPath(std::vector<size_t> path)
    {
        int min = std::numeric_limits<int>::max();
        for (size_t edge: path)
            min = std::min(capacity[edge] - flow[edge], min);
        for (size_t edge: path)
        {
            flow[edge] -= min;
            flow[rev[edge]] += min;
        }
        return min;
    }

    // finds an arbitrary path in a residual network
    std::vector<size_t> findShortestPath()
    {
        DijkstraShortestPaths::SPFinder<size_t, int, int, std::plus<int>, std::less<int>, Extractor, Extractor> finder(
            graph, Extractor(capacity, flow, incident, E_Weight), Extractor(capacity, flow, incident, E_Incident)); 
        finder.calcSPDense(source);
        return finder.getEdgePath(sink);
    }

private: // user data
    const graph::Graph<std::pair<size_t, int> >& network;
    size_t source, sink;

private: // algo stuff
    graph::Graph<size_t> graph;

    std::vector<size_t> incident;
    std::vector<int> capacity;
    std::vector<int> flow;
    std::vector<size_t> rev;
};

#endif // MAXFLOW_HPP
