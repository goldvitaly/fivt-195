#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP

#include <../../Graph/Graph.hpp>
#include <../dijkstra/dijkstra.hpp>
#include <../../incidences/VectorIncidence.hpp>

// написано на коленке, пока нет поддержки пользовательских величин пропускной способности
class MaxflowFinder
{
public:
    MaxflowFinder(const graph::Graph<std::pair<size_t, int> >& network_):
        network(network_)
    {
    }

    int maxFlow(size_t source, size_t sink)
    {
        return -1; // dummy
    }

private:
    // returns value of a minimum edge on the path
    int applyAugPath(std:vector<size_t> path)
    {
    }

    // finds an arbitrary path in a residual network
    std::vector<size_t> findShortestPath()
    {
        return std::vector<size_t>();
    }

private: // user data
    const graph::Graph<std::pair<size_t, int> >& network;

private: // algo stuff
    graph::Graph<size_t> graph;

    std::vector<size_t> incident;
    std::vector<int> capacity;
    std::vector<size_t> flow;
};

#endif // MAXFLOW_HPP
