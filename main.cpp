#include "Graph/Graph.hpp"
#include "incidences/VectorIncidence.hpp"
#include "algorithm/dijkstra/dijkstra.hpp"
#include <iostream>

using namespace graph;
using namespace DijkstraShortestPaths;

typedef std::pair<size_t, int> Edge;

size_t first(Edge e) {return e.first;}
int second(Edge e) {return e.second;}

int main()
{
    Graph<Edge> graph;
    freopen("input.txt", "r", stdin);
    int n, m;
    std::cin >> n >> m;
    for (size_t i = 0; i < (size_t)n; i++)
        graph.addVertex(Graph<Edge>::IncidencePtr(new VectorIncidence<Edge>()));
    for (size_t i = 0; i < (size_t)m; i++)
    {
        int u, v, w;
        std::cin >> u >> v >> w;
        graph.addEdge(--u, std::make_pair(--v, w));
        graph.addEdge(v, std::make_pair(u, w));
    }
    
    SPFinder<Edge> finder(graph, second, first);
    finder.calcSPDense(0);
    for (size_t i = 0; i < graph.size(); i++)
    {
        auto res = finder.getDist(i);
        std::cout << i+1 << ": " << res.reach << " " << res.dist << std::endl;
    }

	return 0;
}
