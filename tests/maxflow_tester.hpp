#ifndef MAXFLOW_TESTER_HPP
#define MAXFLOW_TESTER_HPP

#include <cstdlib>
#include <algorithm>
#include <iostream>

#include "../Graph/Graph.hpp"
#include "../algorithm/maxflow/maxflow.hpp"

using namespace graph;

namespace maxflow_tests
{
typedef std::pair<size_t, int> Edge;

void addRandomPath(Graph<Edge> &graph, int flow)
{
    std::vector<size_t> path;
    generate_n(back_inserter(path), graph.size() - 2, [&path](){return path.size() + 1;});
    path.resize(rand()%path.size());
    path.insert(path.begin(), 0);
    path.push_back(graph.size() - 1);
    for (size_t i = 0; i < path.size() - 1; i++)
        graph.addEdge(path[i], std::make_pair(path[i+1], flow));
}
bool test(int vertexNum, int pathsNum)
{
    Graph<Edge> graph;
    for (int i = 0; i < vertexNum; i++)
        graph.addVertex(Graph<Edge>::IncidencePtr(new VectorIncidence<Edge>()));
    int flow = 0;
    for (int i = 0; i < pathsNum; i++)
    {
        int additionalValue = rand()%100;
        addRandomPath(graph, additionalValue);
        MaxflowFinder finder(graph);
        int newFlow = finder.maxFlow(0, vertexNum - 1);
        if (newFlow != flow + additionalValue)
            return false;
        flow = newFlow;
    }
    return true;
}
} // namespace maxflow_tests

#endif
