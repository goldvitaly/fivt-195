#include "algDeikstra.h"
#include "graph.h"
#include "VertexVector.h"
#include "VertexSet.h"

void make_random_graph(Graph<Vertex>& graph, int numVer)
{
    srand(time(NULL));
    for(int i = 0; i < numVer; i++)
    {
        if(i%2 == 0)
            graph.add_vertex(i, new VertexVector());
        else
            graph.add_vertex(i, new VertexSet());
    }
    for(int i = 0; i < 2*numVer; i++)
    {
        int in_ver = rand() % numVer;
        int out_ver = rand() % numVer;
        graph.add_edge(out_ver, in_ver);
    }
}


int main()
{
    Graph<Vertex> graph;
    make_random_graph(graph, 10);
    AlgDijkstra algDijkstra(graph);
    int ans = algDijkstra.count(0, 3);
    std::cout << ans << std::endl;
    return 0;
}
